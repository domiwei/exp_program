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

        printf("%s", buffer);
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
        FILE *fptr, *temp_fptr;
        read_control callback_data;
        long long size;
	char *json_buffer = "{\"title\": \"2017test\"}";

        fptr = fopen("testfile", "r");
        fseek(fptr, 0, SEEK_END);
        size = ftell(fptr);
        callback_data.fptr = fptr;
        callback_data.remaining = size;
        fseek(fptr, 0, SEEK_SET);

        temp_fptr = fopen("header", "w+");

        curl_global_init(CURL_GLOBAL_ALL);

        /* Fill in the filename field */ 
	curl_formadd(&formpost,
			&lastptr,
			//CURLFORM_CONTENTTYPE, "Content-Type: application/json; charset=UTF-8",
			CURLFORM_CONTENTTYPE, "application/json",
			//CURLFORM_COPYCONTENTS, "{\"title\": \"2017test\"}",
			CURLFORM_BUFFERPTR, json_buffer,
			CURLFORM_BUFFERLENGTH, strlen(json_buffer),
			CURLFORM_END);


        /* Fill in the submit field too, even if this is rarely needed */ 
        curl_formadd(&formpost,
                        &lastptr,
                        //CURLFORM_CONTENTTYPE, "Content-Type: text/plain; charset=UTF-8",
             //           CURLFORM_CONTENTTYPE, "text/plain",
                        //CURLFORM_FILENAME, "testfile",
                        //CURLFORM_COPYNAME, "submit",
                        CURLFORM_STREAM, &callback_data,
                        CURLFORM_CONTENTSLENGTH, (long) size,
                        CURLFORM_END);

        curl = curl_easy_init();
        /* initialize custom header list (stating that Expect: 100-continue is not
           wanted */ 
        headerlist = curl_slist_append(headerlist, buf);
        headerlist = curl_slist_append(headerlist, "Authorization: Bearer ya29.CjDIA0S7ahXfeouWpIXS8v0PId8bTY1J8LuVDz4_g7bCsu_10EUQJ7_4tGGhkm4w8TU");
        //headerlist = curl_slist_append(headerlist, "Content-Type: multipart/related; boundary=test");
	if(curl) {
		/* what URL that receives this POST */ 
		curl_easy_setopt(curl, CURLOPT_URL, "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart");
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, myread);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEHEADER, temp_fptr);

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
	fclose(temp_fptr);
	return 0;
}

