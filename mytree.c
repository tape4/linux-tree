// 2019203032 JeongHoon Lee

#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct file_name_type {
    char *name;
    int is_directory;
} files;

void custom_print(char *base_print, char *file_path, char *file_name, int flag);
void get_dir_files(char *file_path, files *data[], int count_file);
char *get_permission_by_stmode(unsigned int st_mode);
void solution(char *file_path, char *base_print);
void sort_dir_files(files *data[], int n);
int count_dir_files(char *file_path);
char *get_file_size(size_t size);

/* count files */
int file_count = 0;
/* count folders */
int directory_count = 0;

int main() {
    char *base_print = (char *)malloc(sizeof(1));
    base_print[0] = 0;

    DIR *dirp;
    dirp = opendir(".");

    printf(".\n");
    solution(".", base_print);
    printf("\n%d directories, %d files\n", directory_count, file_count);
    return 0;
}

/* sort directory files by name */
void sort_dir_files(files *data[], int n) {
    int step, i;
    files *p_temp_str;
    for (step = 0; step < n; step++) {
        for (i = 0; i < n - step; i++) {
            if (strcmp(data[i]->name, data[i + 1]->name) > 0) {
                p_temp_str = data[i];
                data[i] = data[i + 1];
                data[i + 1] = p_temp_str;
            }
        }
    }
}

/* print file data */
void custom_print(char *base_print, char *file_path, char *file_name,
                  int flag) {
    struct stat st;
    stat(file_path, &st);
    char *new_base_print;
    if (flag) {
        new_base_print = malloc(strlen(base_print) + strlen("└── ") + 1);
        sprintf(new_base_print, "%s%s", base_print, "└── ");
    } else {
        new_base_print = malloc(strlen(base_print) + strlen("├── ") + 1);
        sprintf(new_base_print, "%s%s", base_print, "├── ");
    }
    printf("%s", new_base_print);
    printf("[%ld %ld %s %s %9s]  %s\n", st.st_ino, st.st_dev,
           get_permission_by_stmode(st.st_mode), getpwuid(st.st_uid)->pw_name,
           get_file_size(st.st_size), file_name);
}

void solution(char *file_path, char *base_print) {
    int i;
    int count_file = count_dir_files(file_path);
    char *temp = (char *)malloc(sizeof(char) * 8);
    files *data[count_file];
    get_dir_files(file_path, data, count_file);

    for (i = 0; i < count_file; i++) {
        char *path = malloc(strlen(file_path) + strlen(data[i]->name) + 2);
        sprintf(path, "%s/%s", file_path, data[i]->name);
        if (i == count_file - 1) {
            custom_print(base_print, path, data[i]->name, 1);
            strcpy(temp, "    ");
        } else {
            custom_print(base_print, path, data[i]->name, 0);
            strcpy(temp, "│   ");
        }
        if (data[i]->is_directory) {
            directory_count++;
            char *new_base_print =
                malloc(strlen(base_print) + strlen(temp) + 1);
            sprintf(new_base_print, "%s%s", base_print, temp);
            solution(path, new_base_print);
        } else {
            file_count++;
        }
    }
}

/* get all file names exclude <./> <../> <hide files> */
void get_dir_files(char *file_path, files *data[], int count_file) {
    struct dirent *dentry;
    DIR *dirp = NULL;
    int index = 0;
    if (!file_path)
        return;
    if ((dirp = opendir(file_path)) == NULL)
        return;
    while ((dentry = readdir(dirp))) {
        if (strcmp(dentry->d_name, ".") == 0 ||
            strcmp(dentry->d_name, "..") == 0 || (dentry->d_name)[0] == '.')
            continue;
        int len = strlen(dentry->d_name);
        char *new_file_name = (char *)malloc(sizeof(char) * (len + 1));
        files *newfile = (files *)malloc(sizeof(files));
        strcpy(new_file_name, dentry->d_name);
        newfile->name = new_file_name;
        if (dentry->d_type == DT_DIR) {
            newfile->is_directory = 1;
        } else {
            newfile->is_directory = 0;
        }
        data[index++] = newfile;
    }
    sort_dir_files(data, index - 1);
    closedir(dirp);
}

/* count how many files in directory exclude <./> <../> <hide files> */
int count_dir_files(char *file_path) {
    struct dirent *dentry;
    DIR *dirp = NULL;
    int count = 0;
    if (!file_path)
        return 0;
    if ((dirp = opendir(file_path)) == NULL)
        return 0;
    while ((dentry = readdir(dirp))) {
        if (strcmp(dentry->d_name, ".") == 0 ||
            strcmp(dentry->d_name, "..") == 0 || (dentry->d_name)[0] == '.')
            continue;
        count++;
    }
    closedir(dirp);
    return count;
}

/* get permission information using st_mode of stat */
char *get_permission_by_stmode(unsigned int st_mode) {
    char *result = (char *)malloc(sizeof(char) * 11);
    result[0] = 0;
    (S_ISDIR(st_mode)) ? strcat(result, "d") : strcat(result, "-");
    (st_mode & S_IRUSR) ? strcat(result, "r") : strcat(result, "-");
    (st_mode & S_IWUSR) ? strcat(result, "w") : strcat(result, "-");
    (st_mode & S_IXUSR) ? strcat(result, "x") : strcat(result, "-");
    (st_mode & S_IRGRP) ? strcat(result, "r") : strcat(result, "-");
    (st_mode & S_IWGRP) ? strcat(result, "w") : strcat(result, "-");
    (st_mode & S_IXGRP) ? strcat(result, "x") : strcat(result, "-");
    (st_mode & S_IROTH) ? strcat(result, "r") : strcat(result, "-");
    (st_mode & S_IWOTH) ? strcat(result, "w") : strcat(result, "-");
    (st_mode & S_IXOTH) ? strcat(result, "x") : strcat(result, "-");
    strcat(result, "\0");
    return result;
}

/* get file size using st_size of stat */
char *get_file_size(size_t size) {
    static const char *SIZES[] = {"", "K", "M", "G"};
    size_t div = 0;
    size_t remain = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        remain = (size % 1024);
        div++;
        size /= 1024;
    }
    char *result = (char *)malloc(sizeof(char) * 12);
    result[0] = 0;
    sprintf(result, "%.1f", (float)size + (float)remain / 1024.0);
    strcat(result, SIZES[div]);
    return result;
}
