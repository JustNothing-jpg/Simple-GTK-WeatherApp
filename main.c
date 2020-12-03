#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>
#include "w.c"

GtkWidget *my_entry,*result;

void parse(char *buffer){
     struct json_object *temp_min,*temp_max,*humidity;
     struct json_object *name;
     struct json_object *weather;
     struct json_object *main;
     struct json_object *parsed_json;
     struct json_object *weather_details;
     struct json_object *weather_main;
     struct json_object *weather_description;
     struct json_object *temp;
     parsed_json = json_tokener_parse(buffer);
     json_object_object_get_ex(parsed_json,"weather",&weather);
     weather_details = json_object_array_get_idx(weather,0);
     json_object_object_get_ex(parsed_json,"name",&name);
     json_object_object_get_ex(weather_details,"main",&weather_main);
     json_object_object_get_ex(weather_details,"description",&weather_description);
     json_object_object_get_ex(parsed_json,"main",&main);
     json_object_object_get_ex(main,"temp",&temp);
     json_object_object_get_ex(main,"temp_min",&temp_min);
     json_object_object_get_ex(main,"temp_max",&temp_max);
     json_object_object_get_ex(main,"humidity",&humidity);
    char new_bye[255];
     sprintf(new_bye,"City:%s\nMore:%s\nMin Temp: %dC\nMax Temp: %dC\nHumidity: %s",json_object_get_string(name),json_object_get_string(weather_description),atoi(json_object_get_string(temp_min)) - 273,atoi(json_object_get_string(temp_max)) - 273,json_object_get_string(humidity));
     gtk_label_set_text(GTK_LABEL(result),new_bye);
}

static gboolean destroy_event(GtkWidget *window,gpointer data){
    char *buf = "Exiting ...\n";
    write(STDOUT_FILENO,buf,sizeof(buf));
    write(STDOUT_FILENO,"\n",2);
    gtk_main_quit();
    return FALSE;
}

static void my_func(GtkWidget *widget,char *data){
    char *text = (char *)gtk_entry_get_text(GTK_ENTRY(my_entry));
    char *buffer = malloc(strlen(text));
    strcpy(buffer,text);
    //free(buffer);
    struct memory chunk;
    init_string(&chunk);
    char url[128];
    makeUrl(url,sizeof(url),buffer);
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl,CURLOPT_URL,url);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)&chunk);
    curl_easy_setopt(curl,CURLOPT_FAILONERROR,1L);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
    int res = curl_easy_perform(curl);
    if(res == CURLE_OK){
        char buffer[chunk.size + 1];
        snprintf(buffer,chunk.size + 1,"%s",chunk.response);
        parse(buffer);
    }
    else{
        //fprintf(stderr,"error %s",curl_easy_strerror(res));
        http_response_error(curl_easy_strerror(res),"main");
    }
    curl_easy_cleanup(curl);
    free(buffer);
}

int main(int argc,char *argv[]){
    /* Creating Widgets */
    GtkWidget *window,*button;    
    GtkWidget *hbox;

/*    GdkColor color;
    color.red = 0xffff;
    color.green = 0xffff;
    color.blue = 0;
*/
    /* Running GTK+ */
    gtk_init(&argc,&argv);
    /* Creating Window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_widget_set_size_request(window, 400, 300);
gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
    /* Writing title for Gui */
    gtk_window_set_title(GTK_WINDOW(window),"Simple Weather Gui");
    /* Setting border in window */
    gtk_container_set_border_width(GTK_CONTAINER(window),75);
//    gtk_widget_modify_bg(window,GTK_STATE_NORMAL,&color);
    hbox = gtk_box_new(TRUE,0);
    /* Adding in window our invisible box*/ 
    gtk_container_add(GTK_CONTAINER(window),hbox);
    /* Connecting our destroy function to exit button called "delete-event" */
    g_signal_connect(window,"delete-event",G_CALLBACK(destroy_event),NULL);

    my_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox),my_entry,TRUE,TRUE,0);
    
    result = gtk_label_new("Result");
    gtk_label_set_use_markup (GTK_LABEL (result), TRUE);
    g_object_set (result, "margin", 20, NULL);
    gtk_label_set_line_wrap (GTK_LABEL (result), TRUE);
    gtk_label_set_max_width_chars (GTK_LABEL (result), 30);
    gtk_box_pack_start(GTK_BOX(hbox),result,FALSE,FALSE,0);

    /* Generating label for button */
    button = gtk_button_new_with_label("Just click on me");
    /* Adding button in our box which in window */
    gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,0);
    /* connect our button with our function */
    g_signal_connect(button,"clicked",G_CALLBACK(my_func),"first button");
    /* That shows our window */
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
