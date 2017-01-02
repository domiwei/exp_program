#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

typedef struct {
        FILE *fptr;
        long long remaining;
} read_control;

size_t myread(char *buffer, size_t size, size_t nitems, void *instream)
{
        long long actual_read;
        read_control *ptr = (read_control *)instream;

        if (ptr->remaining <= 0)
                return 0;
        if (size * nitems <= ptr->remaining)
                actual_read = size * nitems;
        else
                actual_read = ptr->remaining;
        fread(buffer, 1, actual_read, ptr->fptr);
        ptr->remaining -= actual_read;

        return actual_read;
}

int main(int argc, char *argv[])
{
        CURL *curl;
        CURLcode res;

        struct curl_httppost *formpost=NULL;
        struct curl_httppost *lastptr=NULL;
        struct curl_slist *headerlist=NULL;
        static const char buf[] = "Expect:";
        FILE *fptr;
        read_control callback_data;
        long long size;

        fptr = fopen("testfile", "r");
        fseek(fptr, 0, SEEK_END);
        size = ftell(fptr);
        callback_data.fptr = fptr;
        callback_data.remaining = size;
        fseek(fptr, 0, SEEK_SET);


        curl_global_init(CURL_GLOBAL_ALL);

        /* Fill in the filename field */ 
        curl_formadd(&formpost,
                        &lastptr,
                        CURLFORM_COPYNAME, "Content-Type: application/json; charset=UTF-8",
                        CURLFORM_COPYCONTENTS, "{\"title\": \"2017test\"}",
                        CURLFORM_END);


        /* Fill in the submit field too, even if this is rarely needed */ 
        curl_formadd(&formpost,
                        &lastptr,
                        CURLFORM_COPYNAME, "Content-Type: text/plain; charset=UTF-8",
                        //CURLFORM_COPYNAME, "submit",
                        CURLFORM_STREAM, &callback_data,
                        CURLFORM_CONTENTLEN, (long) size,
                        CURLFORM_END);

        curl = curl_easy_init();
        /* initialize custom header list (stating that Expect: 100-continue is not
           wanted */ 
        headerlist = curl_slist_append(headerlist, buf);
        headerlist = curl_slist_append(headerlist, "Authorization: Bearer ya29.CjDHA_pr0agzkU3THzqFv1sZRf2LjbxPa4EL6QI-v5r3jXLEVfNYVGWrkt9V_Wz-TjM");
        headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data; boundary=aabbccdd");
                if(curl) {
                        /* what URL that receives this POST */ 
                        curl_easy_setopt(curl, CURLOPT_URL, "https://www.googleapis.com/upload/drive/v2/files?uploadType=multipart");
                        curl_easy_setopt(curl, CURLOPT_READFUNCTION, myread);
                        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
                        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

                        /* Perform the request, res will get the return code */ 
                        res = curl_easy_perform(curl);
                        /* Check for errors */ 
                        if(res != CURLE_OK)
                                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                                                curl_easy_strerror(res));

                        /* always cleanup */ 
                        curl_easy_cleanup(curl);

                        /* then cleanup the formpost chain */ 
                        curl_formfree(formpost);
                        /* free slist */ 
                        curl_slist_free_all(headerlist);
                }
        return 0;
}

