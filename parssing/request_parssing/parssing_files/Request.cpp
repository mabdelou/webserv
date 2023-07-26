#include "Request_utilitis.hpp"

void request_parssing(void)
{
    static std::map<int,s_request_data> data;
    std::map<std::string,std::string> extantion_map;
    extention_map_init(extantion_map);

    for(std::list<gFileInfo>::iterator it = clientInfo.begin(); it != clientInfo.end(); it++)
    {
        if((*it).collected > 0)
        {
            if (data.end() == data.find((*it).id) || (*it).status == __START__)
                ft_init_map_data(data,it);

            if((*it).status == __START__)
            {
                ft_init_data(data,it);
                std::stringstream sstream((*it).buff);
                std::string str;
                if(!get_method_data(data,sstream,it))
                    return ;
                if(!get_path_and_protocol_data(data,sstream,it))
                    return ;
                while(sstream >> str && str != "")
                {
                    if (str == "Host:")
                    {
                        if(!get_host_data(data,sstream,it))
                            return ;
                    }
                    else if(str == "Accept:")
                        get_accept_data(data,sstream,it);
                    else if(str == "Accept-Language:")
                        get_accept_language_data(data,sstream,it);
                    else if(str == "Accept-Encoding:")
                        get_accept_encoding_data(data,sstream,it);
                    if(str == "Content-Type:")
                        get_content_type_data(data,sstream,it);
                    else if(str == "Connection:")
                        get_connection_data(data,sstream,it);
                    else if(str == "Transfer-Encoding:")
                    {
                        if(!get_trasfer_encoding_data(data,sstream,it))
                            return ;
                    }
                    else if(str == "Content-Length:")
                    {
                        data[(*it).id].req_head.is_content_length = true;
                        if(!get_content_length_data(data,sstream,it))
                            return ;
                    }
                    else if(str == "--" + data[(*it).id].req_head.boundary)
                    {
                        get_boundray_files_data(data,sstream,it);
                        break;
                    }
                    else
                        get_another_data(data,sstream,str,it);
                }
                if(data[(*it).id].req_head.method == "GET")
                {
                    end_request_parssing(data,it);
                    return ;
                }
                else if(data[(*it).id].req_head.method == "POST")
                {
                    if(data[(*it).id].req_head.request_body.filename != "" && data[(*it).id].req_head.boundary != "")
                    {
                        if(!write_boundray_body_data_v1(data,it))
                            return ;
                    }
                    else if(data[(*it).id].req_head.transfer_encoding != "" && (*it).collected)
                    {
                        if(!write_chuncked_body_data_v1(data,it,extantion_map))
                            return ;
                    }
                    else if(data[(*it).id].req_head.request_body.content_type == "application/x-www-form-urlencoded")
                    {
                        bool can_save = false;
                        for(int a=0;a<(*it).collected;a++)
                        {
                            if(a+3 < (*it).collected)
                            {
                                if((*it).buff[a] == '\r' && (*it).buff[a+1] == '\n' && (*it).buff[a+2] == '\r' && (*it).buff[a+3] == '\n')
                                {
                                    can_save = true;
                                    a +=4;
                                }
                            }
                            if(can_save)
                            {
                                data[(*it).id].req_head.query_body += (*it).buff[a];
                                // printf("%c", (*it).buff[a]); 
                                data[(*it).id].num_of_bdw++;
                            }
                        }
                        if(data[(*it).id].num_of_bdw>= data[(*it).id].req_head.content_lenght)
                        {
                            (*it).status = __END__;
                            tRequest req;
                            req.id = (*it).id;
                            delete data[(*it).id].out_file;
                            data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                            req.request = data[(*it).id].req_head;
                            req.serverPort  = (*it).serverPort;
                            gRequest.push_back(req);
                            return ;
                        } 
                    }
                    else
                    {
                        tRequest req;
                        req.id = (*it).id;
                        data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                        req.request = data[(*it).id].req_head;
                        req.serverPort  = (*it).serverPort;
                        gRequest.push_back(req);
                        (*it).status = __END__;
                        delete data[(*it).id].out_file;
                        return ;
                    }
                }
                else if (data[(*it).id].req_head.method == "DELETE")
                {
                    end_request_parssing(data,it);
                }
            }
            else if(data[(*it).id].req_head.transfer_encoding != "" && (*it).collected)
            {
                if(!write_chuncked_body_data_v2(data,it))
                    return ;
            }
            else if(data[(*it).id].req_head.request_body.filename != "" && data[(*it).id].req_head.boundary != "")
            {
                if(!write_boundray_body_data_v2(data,it))
                    return ;
            }
        }
    }
}