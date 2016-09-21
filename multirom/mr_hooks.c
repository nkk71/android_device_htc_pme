/*
 * This file contains device specific hooks.
 * Always enclose hooks to #if MR_DEVICE_HOOKS >= ver
 * with corresponding hook version!
 */

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <private/android_filesystem_config.h>

#include <log.h>
#include <util.h>
#include <multirom.h>

#if MR_DEVICE_HOOKS >= 1

int mrom_hook_after_android_mounts(const char *busybox_path, const char *base_path, int type)
{
    // On m8, this fstab file is used to remount system to RO,
    // but with MultiROM, it remounts everything as RO, even /data and /cache
    if(access("/remount.qcom", F_OK) >= 0)
        remove("/remount.qcom");

    // remove mounting from .rc files
    int res = -1;
    int add_dummy = 0;
    char *l;
    char line[1024];
    char path_in[128];
    char path_out[128];
    FILE *in, *out;
    DIR *d;
    struct dirent *dp;

    d = opendir("/");
    if(!d)
        return -1;

    while((dp = readdir(d)))
    {
        if(dp->d_type != DT_REG || !strstr(dp->d_name, ".rc"))
            continue;

        snprintf(path_out, sizeof(path_out), "/%s", dp->d_name);
        snprintf(path_in, sizeof(path_in), "/%s.new", dp->d_name);

        if(rename(path_out, path_in) < 0)
            goto exit;

        in = fopen(path_in, "r");
        if(!in)
            goto exit;

        out = fopen(path_out, "w");
        if(!out)
        {
            fclose(in);
            goto exit;
        }

        while((fgets(line, sizeof(line), in)))
        {
            for(l = line; isspace(*l); ++l);

            if (strncmp(l, "on ", 3) == 0)
                add_dummy = 1;
            else if (strstr(l, "mount ") &&
                (strstr(l, "/data") || strstr(l, "/system") || strstr(l, "/cache")))
            {
                if(add_dummy == 1)
                {
                    add_dummy = 0;
                    fputs("    export DUMMY_LINE_INGORE_IT 1\n", out);
                }

                fputc((int)'#', out);
            }

            fputs(line, out);
        }

        fclose(out);
        fclose(in);
        remove(path_in);

        chmod(path_out, 0750);
    }

    res = 0;
exit:
    closedir(d);
    return res;
}

#endif /* MR_DEVICE_HOOKS >= 1 */

#if MR_DEVICE_HOOKS >= 2
void mrom_hook_before_fb_close(void) { }
#endif

#if MR_DEVICE_HOOKS >= 3
void tramp_hook_before_device_init(void) { }
#endif

#if MR_DEVICE_HOOKS >= 4
int mrom_hook_allow_incomplete_fstab(void)
{
    return 1;
}
#endif

#if MR_DEVICE_HOOKS >= 5
void mrom_hook_fixup_bootimg_cmdline(char *bootimg_cmdline, size_t bootimg_cmdline_cap)
{
}

int mrom_hook_has_kexec(void)
{
    return 0;
}
#endif


#if MR_DEVICE_HOOKS >= 6
static int fork_and_exec(char **cmd, char** env)
{
    pid_t pID = fork();
    if(pID == 0)
    {
        stdio_to_null();
        setpgid(0, getpid());
        execve(cmd[0], cmd, env);
        ERROR("Failed to exec %s: %s\n", cmd[0], strerror(errno));
        _exit(127);
    }
    return pID;
}

static int qseecomd_pid = -1;

void tramp_hook_encryption_setup(void)
{
    // setup additional dirs & files needed by decryption, not currently covered in encryption.c
    remove("/system/etc");
    mkdir_recursive("/system/etc", 0755);
    symlink("/mrom_enc/system/etc", "/system/etc");

    remove("/firmware");
    symlink("/mrom_enc/firmware", "/firmware");

    //
    chmod("/mrom_enc/strace_static", 0775);


    // start qseecomd
    char* cmd[] = {"/mrom_enc/qseecomd", NULL};
    char* env[] = {"LD_LIBRARY_PATH=/mrom_enc", NULL};

    chmod("/dev/qseecom", 0660);
    chown("/dev/qseecom", AID_SYSTEM, AID_DRMRPC);
    chmod("/dev/ion", 0664);
    chown("/dev/ion", AID_SYSTEM, AID_SYSTEM);

    chmod("/mrom_enc/qseecomd", 0755);
    qseecomd_pid = fork_and_exec(cmd, env);
    if (qseecomd_pid == -1)
        ERROR("Failed to fork for qseecomd; should never happen!\n");
    else
        INFO("qseecomd started: pid=%d\n", qseecomd_pid);
}

void tramp_hook_encryption_cleanup(void)
{
    //struct stat info;
    if (qseecomd_pid != -1)
    {
        kill(-qseecomd_pid, SIGTERM); // kill the entire process group
        waitpid(qseecomd_pid, NULL, 0);
    }
    // make sure we're removing our symlink
    //if (lstat("/dev/block/bootdevice", &info) >= 0 && S_ISLNK(info.st_mode))
    //    remove("/dev/block/bootdevice");
    INFO("cleaned up after qseecomd\n");

    remove("/firmware");
    remove("/system/etc");
}

static void replace_tag(char *cmdline, size_t cap, const char *tag, const char *what)
{
    char *start, *end;
    char *str = cmdline;
    char *str_end = str + strlen(str);
    size_t replace_len = strlen(what);

    while((start = strstr(str, tag)))
    {
        end = strstr(start, " ");
        if(!end)
            end = str_end;
        else if(replace_len == 0)
            ++end;

        if(end != start)
        {

            size_t len = str_end - end;
            if((start - cmdline)+replace_len+len > cap)
                len = cap - replace_len - (start - cmdline);
            memmove(start+replace_len, end, len+1); // with \0
            memcpy(start, what, replace_len);
        }

        str = start+replace_len;
    }
}

void mrom_hook_fixup_full_cmdline(char *bootimg_cmdline, size_t bootimg_cmdline_cap)
{
    // force dm-verity to Logging mode to prevent scary "your phone is corrupt" message
    //replace_tag(bootimg_cmdline, bootimg_cmdline_cap, "androidboot.veritymode=", "androidboot.veritymode=logging");
}
#endif
