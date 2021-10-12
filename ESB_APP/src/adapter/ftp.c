#include <stdio.h>
#include <stdlib.h>
#include "ftp.h"

#define PORT 4445
#define BUF_SIZE 2000
#define SIZE 1024

#define HOSTNAME_OR_IP "hostname"
#define USERNAME "username"
#define PASSWORD "password"
//#define FILENAME "filename"
#define TOKEN "token"

void transport_for_ftp_serice_880(bmd *msg)
{

    //CREATE NEW file and insert the necessary (sender, payload) bmd msg into that file
    //take that filename and copy into FILENAME variable
    static int j = 0;
    j++;
    char filename[50];

    //char *file=filename;
    char op[20001];
    snprintf(filename, sizeof(filename), "output_ftp%d.csv", j);
    snprintf(op, sizeof(op), "source\tpayload\n%s\t%s\n", msg->envelop.Sender, msg->payload);
    printf("successfully transformed to CSV, filename=%s\n", filename);
    FILE *fp = fopen(filename, "w"); //open that file
    fprintf(fp, "%s", op);
    fclose(fp);
    char *file = filename; // transformation done

    char *dump_fname = malloc(200 * sizeof(char));
    sprintf(dump_fname, "./.transport_dump/SFTP_%s_%s_%s_%d.csv", HOSTNAME_OR_IP, USERNAME, PASSWORD, j);
    // Open another file for read and one for write
    FILE *f_r = fopen(file, "r");
    FILE *f_w = fopen(dump_fname, "w");

    if (f_r == NULL || f_w == NULL)
        return;

    // Read contents from file
    char c = fgetc(f_r);
    while (c != EOF)
    {
        fputc(c, f_w);
        c = fgetc(f_r);
    }
    fclose(f_w);
    fclose(f_r);
}

void transform_for_ftp_serice_880(bmd *msg)
{

    msg->payload = msg->payload;
    return;
}
