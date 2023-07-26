#include "Request_utilitis.hpp"


void ft_init_map_data(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it)
{
    s_request_data req_data;
    req_data.can_write = false;
    req_data.length = 0;
    req_data.num_of_bdw = 0;
    req_data.req_head.port = 0;
    req_data.req_head.content_lenght = 0;
    req_data.req_head.num_of_body_data_writed = 0;
    req_data.out_file = new std::ofstream;

    data[(*it).id] = req_data;
}
void ft_init_map_boundray_data(std::map<int,s_boundray_data> &boundray_data, std::list<gFileInfo>::iterator &it)
{
    s_boundray_data b_data;
    b_data.bondray_index = 0;
    b_data.old_data.clear();
    b_data.old_return = false;

    boundray_data[(*it).id] = b_data;
}

void ft_init_data(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it)
{
    data[(*it).id].req_head.bad_request = 5;
    data[(*it).id].req_head.is_content_length = false;
    data[(*it).id].length = 0;
    data[(*it).id].num_of_bdw = 0;
    data[(*it).id].req_head.port = 0;
    data[(*it).id].req_head.content_lenght = 0;
    data[(*it).id].req_head.num_of_body_data_writed = 0;
    data[(*it).id].req_head.method = "";
    data[(*it).id].req_head.path = "";
    data[(*it).id].req_head.query_data.clear();
    data[(*it).id].req_head.query_body.clear();
    data[(*it).id].req_head.query_path.clear();
    data[(*it).id].req_head.protocol = "";
    data[(*it).id].req_head.host = "";
    data[(*it).id].req_head.connection = "";
    data[(*it).id].req_head.transfer_encoding = "";
    data[(*it).id].req_head.boundary = "";
    data[(*it).id].req_head.origin = "";
    data[(*it).id].req_head.referer = "";
    data[(*it).id].req_head.upgrade_insecure_request = "";
    data[(*it).id].req_head.sec_fetch_dest = "";
    data[(*it).id].req_head.sec_fetch_mode = "";
    data[(*it).id].req_head.sec_fetch_site = "";
    data[(*it).id].req_head.sec_fetch_user = "";
    data[(*it).id].req_head.accept.clear();
    data[(*it).id].req_head.accept_language.clear();
    data[(*it).id].req_head.accept_encoding.clear();
    data[(*it).id].req_head.request_body.content_disposition = "";
    data[(*it).id].req_head.request_body.filename = "";
    data[(*it).id].req_head.request_body.content_type = "";
    data[(*it).id].req_head.request_body.name = "";
    data[(*it).id].can_write  = false;
}

bool check_if_begin_bondray(std::string begin_boundry, std::list<gFileInfo>::iterator &it ,int index)
{
    static std::map<int,s_boundray_data> boundray_data;
    if (boundray_data.end() == boundray_data.find((*it).id) || (*it).status == __START__)
            ft_init_map_boundray_data(boundray_data,it);
    while(boundray_data[(*it).id].bondray_index < begin_boundry.length() && index < (*it).collected)
    {
        if(begin_boundry[boundray_data[(*it).id].bondray_index++] != (*it).buff[index++])
        {
            boundray_data[(*it).id].bondray_index = 0;
            return(false);
        }
    }
    if(boundray_data[(*it).id].bondray_index == begin_boundry.length())
        boundray_data[(*it).id].bondray_index = 0;
    else
        return(false);
    return(true);
}


bool check_if_end_bondray(std::string end_bondray, std::list<gFileInfo>::iterator &it ,int index)
{
    static std::map<int,s_boundray_data> boundray_data;
    if (boundray_data.end() == boundray_data.find((*it).id) || (*it).status == __START__)
            ft_init_map_boundray_data(boundray_data,it);
    while(boundray_data[(*it).id].bondray_index<end_bondray.length() && index < (*it).collected)
    {
        if((*it).buff[index] == '\n')
            index++;
        if(end_bondray[boundray_data[(*it).id].bondray_index++] != (*it).buff[index++])
        {
            boundray_data[(*it).id].bondray_index = 0;
            return(false);
        }
    }
    if(boundray_data[(*it).id].bondray_index == end_bondray.length())
    {
        boundray_data[(*it).id].bondray_index = 0;
        return(true);
    }
    return(false);
}

bool get_method_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;

    sstream >> string;
    data[(*it).id].req_head.method = string;
    if (data[(*it).id].req_head.method != "GET" && data[(*it).id].req_head.method != "POST"
        && data[(*it).id].req_head.method != "DELETE")
    {
        tRequest req;
        req.id = (*it).id;
        delete data[(*it).id].out_file;
        data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
        data[(*it).id].req_head.bad_request = METHOD_UNK;
        req.request = data[(*it).id].req_head;
        req.serverPort  = (*it).serverPort;
        gRequest.push_back(req);
        (*it).status = __END__;
        return (false);
    }
    return (true);
}

void get_query_data(std::map<int,s_request_data> &data, std::string &string, std::list<gFileInfo>::iterator &it)
{
    (void) data;
    size_t find_ = 0;
    find_ = string.find(".php");
    if(std::string::npos != find_)
        find_ += 4;
    else
    {
        find_ = string.find(".py");
        if(std::string::npos != find_)
            find_ += 3;
    }
    if(std::string::npos != find_)
    {
        if(string[find_] == '/')
        {
            while(string[find_] && string[find_] != '%')
                data[(*it).id].req_head.query_path += string[find_++];
        }
        if(string[find_] == '?')
        {
            while(string[++find_])
                data[(*it).id].req_head.query_data += string[find_];
        }
    }
}
bool get_path_and_protocol_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;

    sstream >> string;
    data[(*it).id].req_head.path = string;
    if(data[(*it).id].req_head.path.length() > 2048)
    {

        tRequest req;
        req.id = (*it).id;
        delete data[(*it).id].out_file;
        data[(*it).id].req_head.bad_request = ENTITY_TOO_LONG;
        req.request = data[(*it).id].req_head;
        req.serverPort  = (*it).serverPort;
        gRequest.push_back(req);
        (*it).status = __END__;
        return (false); // 415 
    }
    get_query_data(data,string,it);
    sstream >> string;
    data[(*it).id].req_head.protocol = string;
    if(data[(*it).id].req_head.protocol != "HTTP/1.1")
    {
        delete data[(*it).id].out_file;
        (*it).status = __END__;
        return (false);
    }
    return (true);
}

bool get_host_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;
    
    sstream >> string;
    bool is_have_port = false;
    for(unsigned int a=0;a < string.length();a++)
        if(string[a] == ':')
            is_have_port = true;
    if(is_have_port == true)
    {
        std::stringstream sstr(string);
        std::getline(sstr, string, ':');
        data[(*it).id].req_head.host = string;
        std::getline(sstr, string, ':');
        try
        {
            for(int a=0;string[a];a++)
                if(string[a] < '0' || string[a] > '9')
                    throw ("[Host:][port:] bad data !");
            int port = stoi(string);
            if(port > UINT16_MAX)
                throw ("[Host:][port:] can't be more then " + std::to_string(UINT16_MAX));
            data[(*it).id].req_head.port = port;
        }
        catch(std::exception &e)
        {
            tRequest req;
            req.id = (*it).id;
            delete data[(*it).id].out_file;
            data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
            data[(*it).id].req_head.bad_request = BAD_REQ;
            req.request = data[(*it).id].req_head;
            req.serverPort  = (*it).serverPort;
            gRequest.push_back(req);
            (*it).status = __END__;
            return (false);
        }
        catch(std::string const &str)
        {
            tRequest req;
            req.id = (*it).id;
            delete data[(*it).id].out_file;
            data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
            data[(*it).id].req_head.bad_request = BAD_REQ;
            req.request = data[(*it).id].req_head;
            req.serverPort  = (*it).serverPort;
            gRequest.push_back(req);
            (*it).status = __END__;
            return (false);  
        }
    }
    else
    {
        data[(*it).id].req_head.port = 80;
        data[(*it).id].req_head.host = string; 
    }
    return (true);
}

void get_accept_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::vector<std::string> acpt;
    std::string string;
    sstream >> string;
    std::stringstream sstr(string);
    while(std::getline(sstr, string, ','))
        acpt.push_back(string);
    data[(*it).id].req_head.accept = acpt;
}

void get_accept_language_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::vector<std::string> acpt_lag;
    std::string string;
    sstream >> string;
    std::stringstream sstr(string);
    while(std::getline(sstr, string, ','))
        acpt_lag.push_back(string);
    data[(*it).id].req_head.accept_language = acpt_lag;
}

void get_content_type_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;
    sstream >> string;
    if(string == "multipart/form-data;")
    {
        std::string remv;
        sstream >> string;
        for(int a=9;string[a];a++)
                remv += string[a];
        data[(*it).id].req_head.boundary = remv;
    }
    else
        data[(*it).id].req_head.request_body.content_type = string;
}

void get_accept_encoding_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::vector<std::string> acpt_enc;
    std::string string;
    while(sstream >> string)
    {
        std::string remv;
        if(string[string.length()-1] == ',')
        {
            for(int a=0;string[a+1];a++)
                remv += string[a];
            acpt_enc.push_back(remv);
        }
        else
        {
            acpt_enc.push_back(string);
            break;
        }
    }   
    data[(*it).id].req_head.accept_encoding = acpt_enc;
}

void get_connection_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;
    sstream >> string;
    data[(*it).id].req_head.connection = string;
}

bool get_trasfer_encoding_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;
    sstream >> string;
    data[(*it).id].req_head.transfer_encoding = string;
    if(string != "chunked")
    {
        tRequest req;
        req.id = (*it).id;
        delete data[(*it).id].out_file;
        data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
        data[(*it).id].req_head.bad_request = NOT_IMPLEMNT;
        req.request = data[(*it).id].req_head;
        req.serverPort  = (*it).serverPort;
        gRequest.push_back(req);
        (*it).status = __END__;
        return (false);
    }
    return (true);
}

void get_another_data(std::map<int,s_request_data> &data, std::stringstream &sstream,std::string &str, std::list<gFileInfo>::iterator &it)
{
    if(str == "Origin:")
    {
        sstream >> str;
        data[(*it).id].req_head.origin = str;
    }
    else if(str == "Referer:")
    {
        sstream >> str;
        data[(*it).id].req_head.referer = str;
    }
    else if(str == "Upgrade-Insecure-Requests:")
    {
        sstream >> str;
        data[(*it).id].req_head.upgrade_insecure_request = str;
    }
    else if(str == "Sec-Fetch-Dest:")
    {
        sstream >> str;
        data[(*it).id].req_head.sec_fetch_dest = str;
    }
    else if(str == "Sec-Fetch-Mode:")
    {
        sstream >> str;
        data[(*it).id].req_head.sec_fetch_mode = str;
    }
    else if(str == "Sec-Fetch-Site:")
    {
        sstream >> str;
        data[(*it).id].req_head.sec_fetch_site = str;
    }
    else if(str == "Sec-Fetch-User:")
    {
        sstream >> str;
        data[(*it).id].req_head.sec_fetch_user= str;
    }
}

bool get_content_length_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;
    sstream >> string;
    try
    {
        for(int a=0;string[a];a++)
            if(string[a] < '0' || string[a] > '9')
                throw ("[Content-Length:] bad data !");
        data[(*it).id].req_head.content_lenght = std::stoi(string);
    }
    catch(std::exception &e)
    {
        tRequest req;
        req.id = (*it).id;
        delete data[(*it).id].out_file;
        data[(*it).id].req_head.num_of_body_data_writed = -1;
        data[(*it).id].req_head.bad_request = BAD_REQ;
        req.request = data[(*it).id].req_head;
        req.serverPort  = (*it).serverPort;
        gRequest.push_back(req);
        (*it).status = __END__;
        return (false);
    }
    catch(const char *str)
    {
        tRequest req;
        req.id = (*it).id;
        delete data[(*it).id].out_file;
        data[(*it).id].req_head.num_of_body_data_writed = -1;
        data[(*it).id].req_head.bad_request = BAD_REQ;
        req.request = data[(*it).id].req_head;
        req.serverPort  = (*it).serverPort;
        gRequest.push_back(req);
        (*it).status = __END__;
        return (false);  
    }
        if((*(getServerPort(gServers,(*it).serverPort))).server_cb_size < data[(*it).id].req_head.content_lenght)
        {
            tRequest req;
            req.id = (*it).id;
            delete data[(*it).id].out_file;
            data[(*it).id].req_head.num_of_body_data_writed = -1;
            data[(*it).id].req_head.bad_request = ENTITY_TO_LARG;
            req.request = data[(*it).id].req_head;
            req.serverPort  = (*it).serverPort;
            gRequest.push_back(req);
            (*it).status = __END__;
            return (false); 
        }
    return (true);
}

void get_boundray_files_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it)
{
    std::string string;
    sstream >> string;
    if(string == "Content-Disposition:")
    {
        std::string remv;
        sstream >> string;
        for(int a=0;string[a+1];a++)
                remv += string[a];
        data[(*it).id].req_head.request_body.content_disposition = remv;
        remv.clear();
        sstream >> string;
        for(int a=6;string[a+2];a++)
            remv += string[a];
        data[(*it).id].req_head.request_body.name = remv;
        remv.clear();
        sstream >> string;
        for(int a=10;string[a+1];a++)
            remv += string[a];
        data[(*it).id].req_head.request_body.filename = remv;
        sstream >> string;
        sstream >> data[(*it).id].req_head.request_body.content_type;
    }
}

void end_request_parssing(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it)
{
    (*it).status = __END__;
    tRequest req;
    req.id = (*it).id;
    delete data[(*it).id].out_file;
    data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
    req.request = data[(*it).id].req_head;
    req.serverPort  = (*it).serverPort;
    gRequest.push_back(req);
}

bool write_boundray_body_data_v1(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it)
{
    int num_of_newline = 0;
    data[(*it).id].req_head.request_body.filename = "/tmp/" + data[(*it).id].req_head.request_body.filename;
    // TODO: open
    (*data[(*it).id].out_file).open(data[(*it).id].req_head.request_body.filename,std::ofstream::binary);
    for(int index=0; index < (*it).collected; index++)
    {
        if(!data[(*it).id].can_write)
            if(check_if_begin_bondray("--" + data[(*it).id].req_head.boundary,it,index))
                data[(*it).id].can_write = true;
        if(data[(*it).id].can_write  && data[(*it).id].num_of_bdw)
        {
            if(check_if_end_bondray("--" + data[(*it).id].req_head.boundary + "--", it, index))
            {
                data[(*it).id].can_write = false;
                (*it).status = __END__;
                tRequest req;
                req.id = (*it).id;
                data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                req.request = data[(*it).id].req_head;
                req.serverPort  = (*it).serverPort;
                gRequest.push_back(req);
                if((*data[(*it).id].out_file).is_open())
                {
                    (*data[(*it).id].out_file).close();
                    delete data[(*it).id].out_file;
                }
                return (false);
            }
        }
        if(data[(*it).id].can_write)
        {
            if(num_of_newline > 3)
            {
                // TODO: write
                (*data[(*it).id].out_file) << (*it).buff[index];
                if(data[(*it).id].num_of_bdw < INT32_MAX)
                    data[(*it).id].num_of_bdw++;
                if(data[(*it).id].num_of_bdw >= data[(*it).id].req_head.content_lenght)
                {
                    (*it).status = __END__;
                    tRequest req;
                    req.id = (*it).id;
                    data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                    req.request = data[(*it).id].req_head;
                    req.serverPort  = (*it).serverPort;
                    gRequest.push_back(req);
                    if((*data[(*it).id].out_file).is_open())
                    {
                        (*data[(*it).id].out_file).close();
                        delete data[(*it).id].out_file;
                    }
                    return (false);
                }
            }
            if((*it).buff[index] == '\n')
                num_of_newline++;
        }
    }
    return (true);
}

bool write_chuncked_body_data_v1(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it,std::map<std::string,std::string> &content_types)
{
    std::string string;
    uint16_t num_of_newline = 0;
    std::stringstream sstr((*it).buff);
    time_t     now = time(0);
    data[(*it).id].req_head.request_body.filename = "/tmp/" + std::to_string(now) + "." + content_types[data[(*it).id].req_head.request_body.content_type];
    // TODO: open
    (*data[(*it).id].out_file).open(data[(*it).id].req_head.request_body.filename, std::ofstream::binary);
    while(std::getline(sstr, string, '\n'))
    {
        if(string == "\r")
        {
            sstr >> std::hex >> data[(*it).id].length;
            if(data[(*it).id].length == 0)
            {
                (*it).status = __END__;
                tRequest req;
                req.id = (*it).id;
                data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                req.request = data[(*it).id].req_head;
                req.serverPort  = (*it).serverPort;
                gRequest.push_back(req);
                if((*data[(*it).id].out_file).is_open())
                {
                    (*data[(*it).id].out_file).close();
                    delete data[(*it).id].out_file;
                }
                return (false);
            }
            break;
        }
    }
    for(int index=0;index<(*it).collected;index++)
    {
        if((*it).buff[index] == '\n' && (*it).buff[index+1] == '\r')
            data[(*it).id].can_write = true;
        if(data[(*it).id].can_write && data[(*it).id].length)
        {
            if(num_of_newline > 2)
            {
                // TODO: write
                // mabdelou_del
                (*data[(*it).id].out_file) << (*it).buff[index];
                data[(*it).id].length--;
                if(data[(*it).id].num_of_bdw < INT32_MAX)
                    data[(*it).id].num_of_bdw++;
                if(data[(*it).id].length == 0)
                {
                    (*it).status = __END__;
                    tRequest req;
                    req.id = (*it).id;
                    data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                    req.request = data[(*it).id].req_head;
                    req.serverPort  = (*it).serverPort;
                    gRequest.push_back(req);
                    if((*data[(*it).id].out_file).is_open())
                    {
                        (*data[(*it).id].out_file).close();
                        delete data[(*it).id].out_file;
                    }
                    return (false);
                }
            }
            if((*it).buff[index] == '\n')
                num_of_newline++;
        }
    }
    return (true);
}

bool write_chuncked_body_data_v2(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it)
{
    // bool hex_var = false;

    static std::string string;
    if(data[(*it).id].length == 0)
    {
        int index = 0;
        for(;index<(*it).collected;index++)
        {
            if((*it).buff[index] == '\n')
                string.clear();
            else if((*it).buff[index] == '\r' && (*it).buff[index+1] == '\n')
            {
                std::stringstream sstr(string);
                sstr >> std::hex >> data[(*it).id].length;        
                string.clear();
                break;
            }
            string += (*it).buff[index];
        }
        if(data[(*it).id].length == 0)
        {
            (*it).status = __END__;
            tRequest req;
            req.id = (*it).id;
            data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
            req.request = data[(*it).id].req_head;
            req.serverPort  = (*it).serverPort;
            gRequest.push_back(req);
            if((*data[(*it).id].out_file).is_open())
            {
                (*data[(*it).id].out_file).close();
                delete data[(*it).id].out_file;
            }
            return (false);
        }
        else
        {
            index += 2;
            for(;index<(*it).collected && data[(*it).id].length>0;index++)
            {
                // TODO: write
                // mabdelou_del
                (*data[(*it).id].out_file) << (*it).buff[index];
                data[(*it).id].length--;
                if(data[(*it).id].num_of_bdw < INT32_MAX)
                    data[(*it).id].num_of_bdw++;
            }
        }
    }
    else
    {
        int index = 0;
        for(;index<(*it).collected && data[(*it).id].length>0;index++)
        {
            // TODO: write
            // mabdelou_del
            (*data[(*it).id].out_file) << (*it).buff[index];
            data[(*it).id].length--;
            if(data[(*it).id].num_of_bdw < INT32_MAX)
                data[(*it).id].num_of_bdw++;
        }
        if(data[(*it).id].length == 0 && index < (*it).collected)
        {
            string.clear();
            index += 2;
            if((*it).buff[index] == '\r' && (*it).buff[index+1] == '\n')
                index += 2;
            for(;index<(*it).collected;index++)
            {
                if((*it).buff[index] == '\r' && (*it).buff[index+1] == '\n')
                {
                    std::stringstream sstr(string);
                    sstr >> std::hex >> data[(*it).id].length;
                    string.clear();
                    break;
                }
                string += (*it).buff[index];
            }
            index += 2;
            if(index < (*it).collected)
            {
                for(;index<(*it).collected && data[(*it).id].length>0;index++)
                {
                    // TODO: write
                    // mabdelou_del
                    (*data[(*it).id].out_file) << (*it).buff[index];
                    data[(*it).id].length--;
                    if(data[(*it).id].num_of_bdw < INT32_MAX)
                        data[(*it).id].num_of_bdw++;
                }
            }
        }
    }
    return(true);
}

bool write_boundray_body_data_v2(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it)
{
    data[(*it).id].can_write = true;
    for(int index=0; index < (*it).collected; index++)
    {
        if(data[(*it).id].can_write)
        {
            if(check_if_end_bondray("--" + data[(*it).id].req_head.boundary + "--", it, index))
            {
                data[(*it).id].can_write = false;
                (*it).status = __END__;
                tRequest req;
                req.id = (*it).id;
                data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                req.request = data[(*it).id].req_head;
                req.serverPort  = (*it).serverPort;
                gRequest.push_back(req);
                if((*data[(*it).id].out_file).is_open())
                {
                    (*data[(*it).id].out_file).close();
                    delete data[(*it).id].out_file;
                }
                
                return (false);
            }
            else
                data[(*it).id].can_write = true;
        }
        if(data[(*it).id].can_write)
        {
            // TODO: write
            (*data[(*it).id].out_file) << (*it).buff[index];
            data[(*it).id].num_of_bdw++;
            if(data[(*it).id].num_of_bdw >= data[(*it).id].req_head.content_lenght)
            {
                (*it).status = __END__;
                tRequest req;
                req.id = (*it).id;
                data[(*it).id].req_head.num_of_body_data_writed = data[(*it).id].num_of_bdw;
                req.request = data[(*it).id].req_head;
                req.serverPort  = (*it).serverPort;
                gRequest.push_back(req);
                if((*data[(*it).id].out_file).is_open())
                {
                    (*data[(*it).id].out_file).close();
                    delete data[(*it).id].out_file;
                }
                return (false);
            }
        }
    }
    return (true);
}

void extention_map_init(std::map<std::string,std::string> &content_types)
{
    content_types["text/html"] = "html";
	content_types["text/css"] = "css";
	content_types["application/xml"] = "xml";
	content_types["image/gif"]= "gif";
	content_types["image/jpeg"]= "jpeg";
	content_types["image/png"]= "png";
	content_types["application/javascript"]= "js";
	content_types["application/atom+xml"]= "atom";
	content_types["application/rss+xml"]= "rss";
	
	content_types["text/mathml"]= "mml";
	content_types["text/plain"]= "txt";
	content_types["text/vnd.sun.j2me.app-descriptor"]= "jad";
	content_types["text/vnd.wap.wml"]= "wml";
	content_types["text/x-component"]= "htc";
	
	content_types["image/tiff"]= "tiff";
	content_types["image/vnd.wap.wbmp"]= "wbmp";
	content_types["image/x-icon"]= "ico";
	content_types["image/x-jng"]= "jng";
	content_types["image/x-ms-bmp"]= "bmp";
	content_types["image/svg+xml"]= "svg";
	content_types["image/svg+xml"]= "svgz";
	content_types["image/webp"]= "webp";

	content_types["application/font-woff"] = "woff";
	content_types["application/java-archive"] = "jar";
	content_types["application/json"] = "json";
	content_types["application/mac-binhex40"] = "hqx";
	content_types["application/msword"] = "doc";
	content_types["application/pdf"] = "pdf";
	content_types["application/postscript"] = "ps";
	content_types["application/rtf"] = "rtf";
	content_types["application/vnd.apple.mpegurl"] = "m3u8";
	content_types["application/vnd.ms-excel"] = "xls";
	content_types["application/vnd.ms-fontobject"] = "eot";
	content_types["application/vnd.ms-powerpoint"] = "ppt";
	content_types["application/vnd.wap.wmlc"] = "wmlc";
	content_types["application/vnd.google-earth.kml+xml"] = "kml";
	content_types["application/vnd.google-earth.kmz"] = "kmz";
	content_types["application/x-7z-compressed"] = "7z";
	content_types["application/x-cocoa"] = "cco";
	content_types["application/x-java-archive-diff"] = "jardiff";
	content_types["application/x-java-jnlp-file"] = "jnlp";
	content_types["application/x-makeself"] = "run";
	content_types["application/x-perl"] = "pl";
	content_types["application/x-pilot"] = "prc";
	content_types["application/x-rar-compressed"] = "rar";
	content_types["application/x-redhat-package-manager"] = "rpm";
	content_types["application/x-sea"] = "sea";
	content_types["application/x-shockwave-flash"] = "swf";
	content_types["application/x-stuffit"] = "sit";
	content_types["application/x-tcl"] = "tcl";
	content_types["application/x-tcl"] = "tk";
	content_types["application/x-x509-ca-cert"] = "cert";
	content_types["application/x-xpinstall"] = "xpi";
	content_types["application/xhtml+xml"] = "xhtml";
	content_types["application/xspf+xml"] = "xspf";
	content_types["application/zip"] = "zip";

	content_types["application/octet-stream"] = "bin";
	content_types["application/octet-stream"] = "exe";
	content_types["application/octet-stream"] = "dll";
	content_types["application/octet-stream"] = "deb";
	content_types["application/octet-stream"] = "dmg";
	content_types["application/x-iso9660-image"] = "iso";
	content_types["application/octet-stream"] = "img";
	content_types["application/octet-stream"] = "msp";
	content_types["application/octet-stream"] = "msm";
	content_types["application/octet-stream"] = "msi"; 

	content_types["application/vnd.openxmlformats-officedocument.wordprocessingml.document"] = "docx";
	content_types["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] = "xlsx";
	content_types["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = "pptx";

	content_types["audio/midi"] = "kar";
	content_types["audio/midi"] = "mid";
	content_types["audio/midi"] = "midi";
	content_types["audio/mpeg"] ="mp3" ;
	content_types[ "audio/ogg"] = "ogg";
	content_types[ "audio/x-m4a"] = "m4a";
	content_types["audio/x-realaudio"] = "ra";

	content_types["video/3gpp"] = "3gpp";
	content_types["video/mp2t"] = "ts";
	content_types["video/mp4"] = "mp4";
	content_types["video/mpeg"] = "mpeg";
	content_types["video/mpeg"] = "mpg";
	content_types["video/quicktime"] = "mov";
	content_types["video/webm"] = "webm";
	content_types["video/x-flv"] = "flv";
	content_types["video/x-m4v"] = "m4v";
	content_types["video/x-mng"] = "mng";
	content_types["video/x-ms-asf"] = "asx";
	content_types["video/x-ms-asf"] = "asf";
	content_types["video/x-ms-wmv"] = "wmv";
	content_types["video/x-msvideo"] = "avi";
}