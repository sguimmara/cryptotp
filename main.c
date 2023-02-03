#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdnoreturn.h>

#include <sys/stat.h>

void print_usage()
{
    puts("cryptotp <file> <keyfile>");
    puts("");
    puts("  <file>      the file to encrypt");
    puts("  <keyfile>   the file that contains the key");
    puts("");
    puts("note: <file> and <keyfile> must be of the same size.");
}

noreturn void exit_with_file_error(char *path)
{
    fprintf(stderr, "%s: %s\n", path, strerror(errno));
    exit(1);
}

FILE *fopen_checked(char *path, char *mode)
{
    FILE *result = fopen(path, mode);
    if (!result)
    {
        exit_with_file_error(path);
    }

    return result;
}

struct stat fstat_checked(FILE *stream, char *path)
{
    struct stat st;
    if (fstat(fileno(stream), &st) != 0)
    {
        exit_with_file_error(path);
    }

    return st;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        print_usage();
        return 1;
    }

    char *textfile = argv[1];
    char *keyfile = argv[2];

    FILE *textfd = fopen_checked(textfile, "rb");
    FILE *otpfd = fopen_checked(keyfile, "rb");

    struct stat ptstat = fstat_checked(textfd, textfile);
    struct stat keystat = fstat_checked(otpfd, keyfile);

    if (ptstat.st_size != keystat.st_size)
    {
        fputs("error: OTP and plain text should have the same size.", stderr);
        return 1;
    }

    size_t size = ptstat.st_size;

    char *plaintext = malloc(size);
    char *otp = malloc(size);

    if (fread(plaintext, sizeof(char), size, textfd) != size)
    {
        fputs("error: could not read plaintext file.", stderr);
    }

    if (fread(otp, sizeof(char), size, otpfd) != size)
    {
        fputs("error: could not read key file.", stderr);
    }

    char *ciphertext = malloc(size);

    for (size_t i = 0; i < size; i++)
    {
        ciphertext[i] = plaintext[i] ^ otp[i];
    }

    fwrite(ciphertext, 1, size, stdout);

    fclose(textfd);
    fclose(otpfd);

    free(plaintext);
    free(otp);
    free(ciphertext);

    return 0;
}
