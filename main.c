
#include <ctype.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define apr_isxdigit(c) (isxdigit(((unsigned char)(c))))

static char x2c(const char *what)
{
    char digit;

#if !APR_CHARSET_EBCDIC
    digit = ((what[0] >= 'A') ? ((what[0] & 0xdf) - 'A') + 10
             : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10
              : (what[1] - '0'));
#else /*APR_CHARSET_EBCDIC*/
    char xstr[5];
    xstr[0]='0';
    xstr[1]='x';
    xstr[2]=what[0];
    xstr[3]=what[1];
    xstr[4]='\0';
    digit = apr_xlate_conv_byte(ap_hdrs_from_ascii,
                                0xFF & strtol(xstr, NULL, 16));
#endif /*APR_CHARSET_EBCDIC*/
    return (digit);
}


static int unescape_url(char *url, const char *forbid, const char *reserved)
{
    int badesc, badpath;
    char *x, *y;

    badesc = 0;
    badpath = 0;

    if (url == NULL) {
        //return OK;
        return 0;
    }
    /* Initial scan for first '%'. Don't bother writing values before
     * seeing a '%' */
    //printf("oooffff\n");
    y = strchr(url, '%');
    //printf("oooffff\n");
    if (y == NULL) {
        //return OK;
        return 0;
    }
    for (x = y; *y; ++x, ++y) {
        //printf("start of loop\n");
        if (*y != '%') {
            *x = *y;
        }
        else {
            if (!apr_isxdigit(*(y + 1)) || !apr_isxdigit(*(y + 2))) {
                badesc = 1;
                *x = '%';
            }
            else {
                char decoded;
                decoded = x2c(y + 1);
                //if ((decoded == '\0')
                //    || (forbid && strchr(forbid, decoded))) {
                if (decoded == '\0') {
                    badpath = 1;
                    *x = decoded;
                    y += 2;
                }
                //else if (reserved && strchr(reserved, decoded)) {
                //    *x++ = *y++;
                //    *x++ = *y++;
                //    *x = *y;
                //}
                else {
                    *x = decoded;
                    y += 2;
                }
            }
        }
    }
    *x = '\0';
    if (badesc) {
        //return HTTP_BAD_REQUEST;
        return 2;
    }
    else if (badpath) {
        //return HTTP_NOT_FOUND;
        return 1;
    }
    else {
        //return OK;
        return 0;
    }
}

/*
int main(int argc, char** argv) {

    char* input_url[1000];
    int return_val;
    fgets(input_url, 999, stdin);


    // static int unescape_url(char *url, const char *forbid, const char *reserved)

    return_val = unescape_url(input_url, NULL, NULL);
    printf("Return value: %d\n", return_val);

    printf("Decoded stuff: %s\n", input_url);

    return 0;
}
*/

int fuzzone(const uint8_t*data, size_t size) {
    int return_val;
    char stuff[100];
    if (size < 100) {
        memcpy(stuff, data, size);
    }
    else {
        memcpy(stuff, data, 99);
    }
    //memcpy(stuff, data, 999);

    return_val = unescape_url(stuff, NULL, NULL);
    return return_val;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  fuzzone(Data, Size);
  return 0;  // Values other than 0 and -1 are reserved for future use.
}

