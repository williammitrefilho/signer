#include <gerio.h>

struct gerio_client* new_gerio_client(){
    
    struct gerio_client *client = (struct gerio_client*)malloc(sizeof(struct gerio_client));
    client->tlsClient = 0;
    client->logged_in = 0;
    return client;
}

unsigned short gerio_login(struct gerio_client *client, unsigned char *hostname, unsigned char *portnum, unsigned char *usuario, unsigned char *senha){
    
    client->tlsClient = tls_connect(hostname, portnum);
    if(!client->tlsClient){
        
        return 1;
    }
    if(tls_handshake(client->tlsClient, hostname, portnum)){
        
        printf("erro handshake\n");
        tls_free_client(client->tlsClient);
        return 1;
    }

    unsigned char url[] = "/pdvgct/?c=index&op=acesso", method[] = "POST";
    unsigned char full_url[256], s_request[512], s_request_data[128], s_ct_len[16];
    unsigned short req_len = 0, req_data_len = 0;
    sprintf(full_url, "%s%s", hostname, url);
    sprintf(s_request_data, "usuario=%s&senha=%s", usuario, senha);

    while(s_request_data[req_data_len] != 0x00)
        req_data_len++;
        
    sprintf(s_ct_len, "%d", req_data_len);

//    printf(full_url);printf("\n");
    
    struct http_request *request = new_http_request(method, full_url);
    add_request_header(request, "Host", hostname);
    add_request_header(request, "Content-type", "application/x-www-form-urlencoded");
    add_request_header(request, "Content-length", s_ct_len);
    
    request->data = s_request_data;
    request->data_len = req_data_len;
    
    build_http_request(request, s_request, &req_len);

    tls_send_application_data(client->tlsClient, s_request, req_len);
    if(tls_receive_application_data(client->tlsClient)){
        
        printf("erro appdata\n");
    }
    else{
        
//        printf(s_request);printf("\n");
//        printchars(client->tlsClient->application_data, client->tlsClient->application_data_len);printf("\n");
        struct http_request *response = parse_http_response(client->tlsClient->application_data, client->tlsClient->application_data_len);
        for(int i = 0; i < response->n_headers; i++){
            
//            printf("header:%s=%s\n", response->headers[i]->name, response->headers[i]->content);
            if(!memcmp(response->headers[i]->name, "set-cookie", response->headers[i]->name_len)){
                
                struct http_header *header = response->headers[i];
                unsigned short c1 = 0, name_len = 0;
                unsigned char *pcookie = header->content;
                while(c1 < header->content_len && pcookie[c1] != '=')
                    c1++;
                
                name_len = c1;
                unsigned char name[name_len+1];
                memcpy(name, pcookie, name_len);
                name[name_len] = 0x00;
                pcookie += name_len+1;
                c1 = 0;
                while(pcookie[c1] != ';' && c1 < (header->content_len - name_len - 1))
                    c1++;
                
                unsigned char value[c1+1];
                value[c1] = 0x00;
                memcpy(value, pcookie, c1);
//                printf("valor:%s\n", value);
                memcpy(client->session_id, value, c1+1);
                client->logged_in = 1;
            }
        }
        free_http_request(response);
    }
    sprintf(client->hostname, "%s", hostname);
    request->data = 0;
    free_http_request(request);
    
    tls_free_client(client->tlsClient);
    client->tlsClient = 0;
    return 0;
}

unsigned short gerio_get_transacao(struct gerio_client *client, struct gerio_transacao *transacao){
    
    if(!client->logged_in)
        return 1;

    client->tlsClient = tls_connect(client->hostname, "443");
    if(tls_handshake(client->tlsClient, client->hostname, "443")){
        
        tls_free_client(client->tlsClient);
        client->tlsClient = 0;
        return 1;    
    }
    
    unsigned char full_url[256], s_request[512], s_cookie[128];
    unsigned short s_req_len = 0;
    sprintf(full_url, "%s%s", client->hostname, "/pdvgct/?c=sinc&op=getNova&aj=sim");
//    printf(full_url);
    struct http_request *request = new_http_request("GET", full_url);
    
    add_request_header(request, "Host", client->hostname);
    add_request_header(request, "Accept-Encoding", "");
    sprintf(s_cookie, "id_sessao=%s", client->session_id);
    add_request_header(request, "Cookie", s_cookie);
    
    build_http_request(request, s_request, &s_req_len);
    free_http_request(request);
    printchars(s_request, s_req_len);
    tls_send_application_data(client->tlsClient, s_request, s_req_len);
    if(tls_receive_application_data(client->tlsClient)){
        
        return 2;
    }
    printchars(client->tlsClient->application_data, client->tlsClient->application_data_len);
    struct http_request *response = parse_http_response(client->tlsClient->application_data, client->tlsClient->application_data_len);
    unsigned char *data = response->data;
    unsigned short data_len = 0;
    
    struct http_header *transfer_encoding = get_request_header(response, "transfer-encoding");
    if(transfer_encoding){

        unsigned char tlength[32];
        unsigned short length = 0, offset = 0;
        read_line(response->data, response->data_len, tlength, 32, 0, &offset);
        sscanf(tlength, "%x", &length);
        data_len = length;
        data += offset;
        printf("data(%d, %d):", response->data, offset);
        for(int i = 0; i < length; i++){
            
            printf("%02X", data[i]);
        }
        printf("\n");
    }
    else{
        
        data_len = response->data_len;
    }   
    
    if(data_len == 10){
        
        unsigned short status = data[0] << 8 | data[1];
        unsigned long  tid = (data[2] << 24)
                                |(data[3] << 16)
                                |(data[4] << 8)
                                |data[5],
                        valor = (data[6] << 24)
                                |(data[7] << 16)
                                |(data[8] << 8)
                                |data[9];

        transacao->tid = tid;
        transacao->valor = valor;
    }
    else{
        
        return 1;
    }
    free_http_request(response);
    tls_free_client(client->tlsClient);
    client->tlsClient = 0;
    return 0;

}

unsigned short gerio_set_transacao(struct gerio_client *client, struct gerio_transacao *transacao){
    
    if(!client->logged_in)
        return 1;
        
    client->tlsClient = tls_connect(client->hostname, "443");
    
    if(tls_handshake(client->tlsClient, client->hostname, "443")){
        
        tls_free_client(client->tlsClient);
        client->tlsClient = 0;
        return 1;    
    }
    
    unsigned char full_url[256], s_request[512], s_cookie[128];
    unsigned short s_req_len = 0;
    sprintf(full_url, "%s/pdvgct/?c=sinc&op=setStatus&aj=sim&tid=%d&status=%d", client->hostname, transacao->tid, transacao->status);
    printf(full_url);
    struct http_request *request = new_http_request("GET", full_url);
    
    add_request_header(request, "Host", client->hostname);
    add_request_header(request, "Accept-Encoding", "");
  
    sprintf(s_cookie, "id_sessao=%s", client->session_id);
    add_request_header(request, "Cookie", s_cookie);
    
    build_http_request(request, s_request, &s_req_len);
    free_http_request(request);
    printchars(s_request, s_req_len);
    tls_send_application_data(client->tlsClient, s_request, s_req_len);
    tls_receive_application_data(client->tlsClient);
    printchars(client->tlsClient->application_data, client->tlsClient->application_data_len);
    
    tls_free_client(client->tlsClient);
    client->tlsClient = 0;
    return 0;

}

unsigned short free_gerio_client(struct gerio_client *client){
    
    if(client->tlsClient)
        tls_free_client(client->tlsClient);
    return 0;
}

unsigned short gerio_cancelar_tef(struct gerio_transacao *transacao){
    
    const unsigned char msg_format[] = "000-000 = NCN\r\n001-000 = %d\r\n004-000 = 0\r\n706-000 = 4\r\n716-000 = SBMATE AUTOMACAO\r\n733-000 = 100\r\n999-999 = 0\r\n";
    FILE *arq = fopen("C:/tef_dial/req/intpos.001", "wb");
    if(arq){
        
        unsigned char msg[256];
        unsigned short msg_len = 0;
        sprintf(msg, msg_format, transacao->tid, transacao->valor);
        while(msg[msg_len] != 0x00){
            
            msg_len++;
        }
        fwrite(msg, 1, msg_len, arq);
        fclose(arq);
    }
    
    return 0;
}

unsigned short gerio_confirmar_tef(struct gerio_transacao *transacao){
    
    const unsigned char msg_format[] = "000-000 = CNF\r\n001-000 = %d\r\n004-000 = 0\r\n706-000 = 4\r\n716-000 = SBMATE AUTOMACAO\r\n733-000 = 100\r\n999-999 = 0\r\n";
    FILE *arq = fopen("C:/tef_dial/req/intpos.001", "wb");
    if(arq){
        
        unsigned char msg[256];
        unsigned short msg_len = 0;
        sprintf(msg, msg_format, transacao->tid, transacao->valor);
        while(msg[msg_len] != 0x00){
            
            msg_len++;
        }
        fwrite(msg, 1, msg_len, arq);
        fclose(arq);
    }
    
    return 0;
}

unsigned short gerio_transacao_para_tef(struct gerio_transacao *transacao){
    
    const unsigned char msg_format[] = "000-000 = CRT\r\n001-000 = %d\r\n003-000 = %d\r\n004-000 = 0\r\n706-000 = 4\r\n716-000 = SBMATE AUTOMACAO\r\n733-000 = 100\r\n999-999 = 0\r\n";
    FILE *arq = fopen("C:/tef_dial/req/intpos.001", "wb");
    if(arq){
        
        unsigned char msg[256];
        unsigned short msg_len = 0;
        sprintf(msg, msg_format, transacao->tid, transacao->valor);
        while(msg[msg_len] != 0x00){
            
            msg_len++;
        }
        fwrite(msg, 1, msg_len, arq);
        fclose(arq);
    }
    
    return 0;
}

unsigned short gerio_tef_para_transacao(struct gerio_transacao *transacao){
    
    FILE *arq = fopen("C:/tef_dial/resp/intpos.001", "rb+");
    if(!arq)
        return 1;
    
    unsigned char line[256], *pline = line;
    pline += 10;
    while(!feof(arq)){
        
        fgets(line, 256, arq);
        if(memcmp("009-000", line, 7) == 0){
            
            printf("status:%s\n", pline);
            if(pline[0] == 0x30)
                transacao->status = 1;
        }
        printf(line);
    }
//    transacao->status = 1;
    fclose(arq);
    return 0;
}