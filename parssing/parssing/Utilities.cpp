#include "Parssing.hpp"
#include "Utilities.hpp"

void err1(std::string const &str)
{
    throw(str + C_WHITE);
}

bool ft_check_config_file_name(char *config_file_name)
{
    unsigned int config_length;

    config_length = ft_strlen(config_file_name);
    if(config_file_name[config_length-1] == 'f'
    && config_file_name[config_length-2] == 'n'
    && config_file_name[config_length-3] == 'o'
    && config_file_name[config_length-4] == 'c'
    && config_file_name[config_length-5] == '.')
        return(1);
    return(0);
}

unsigned int ft_calc_num_of_spaces_in_line(std::string const &str)
{
    unsigned int num_of_spaces = 0;
    for(unsigned int a = 0; str[a]; a++)
    {
        if(str[a] == ' ')
            num_of_spaces += 1;
        else if (str[a] == '\t')
            num_of_spaces += 4;
        else
            break;
    }
    return(num_of_spaces);
}

uint16_t ft_check_if_listen_data_valid(s_variables *vars, std::string const &data)
{
    int port;
    for(unsigned int a = 0; data[a]; a++)
        if(data[a] < 48 || data[a] > 57)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[listen:] syntax unknown");
    try
    { 
        port = stoi(data);
    }
    catch(std::exception &e)
    {
       err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[listen:] bad port number");
    }
    if(port > 65535)
         err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[listen:] port can't be more then 65535");
    if(port == 0)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[listen:] [" + std::to_string(port) + "] this port is invalid !");
    return(port);
}

uint32_t ft_check_if_cb_body_data_valid(s_variables *vars, std::string const &data, int num_of_parm)
{
    unsigned int body_size;
    for(unsigned int a = 0; data[a]; a++)
        if(data[a] < 48 || data[a] > 57)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[client_body_size:] syntax unknown");
    try
    { 
        body_size = stoi(data);
    }
    catch(std::exception &e)
    {
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[client_body_size:] port can't be more then 2147483647");
    }
    if(data == "")
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[client_body_size:] data not exist");
    if(body_size > 2147483647)
         err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[client_body_size:] port can't be more then 2147483647");
    else if(num_of_parm > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[client_body_size:] have more then one parameter");
    return(body_size);
}


void ft_put_server_listen_data(s_variables *vars, s_server &server, s_server_variables &server_vars,std::vector<s_server> &servers_list)
{
    static int num_of_times_call_funtion = 0;
    static int num_of_server_last_time = 1;
    std::string host;
    std::string s_port;
    uint16_t    port = 0;
    int num_of_parm = 0;
    int num_of_double_point = 0;
    bool another_value = false;

    if(vars->num_of_server_know > num_of_server_last_time)
    {
        ++num_of_server_last_time;
        num_of_times_call_funtion = 0;
    }
    if(++num_of_times_call_funtion > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[listen:] duplicated detected");

    std::stringstream sstr(vars->config_file_line);
    sstr >> vars->string;
    while(sstr >> vars->string)
    {
        for(unsigned int a=0;a < vars->string.length();a++)
            if(vars->string[a] == ':')
                num_of_double_point++;
        if(!num_of_double_point)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[listen:] should be 'host:port'");
        else if(vars->string[0] == ':')
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[listen:] should be 'host:port'");
        else if(vars->string[vars->string.length()-1] == ':')
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[listen:] should be 'host:port'");
        else if(num_of_double_point > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[listen:] have more then one ':'");
        for(unsigned int a=0;a < vars->string.length();a++)
        {
            if(vars->string[a] == ':')
                another_value = true;
            else if(!another_value)
                host += vars->string[a];
            else
                s_port += vars->string[a];
        }
        num_of_parm++;
    }
    if(!num_of_parm)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[listen:] have no parameter");
    if(num_of_parm > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[listen:] have more then one parameter");
    port = ft_check_if_listen_data_valid(vars, s_port);
    for(std::vector<s_server>::iterator it = servers_list.begin() ; it < servers_list.end() ; it++)
        if(port == (*it).server_listen)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[listen:] number of port duplicated detected");
    for(unsigned int a=0;a < host.length();a++)
        if((host[a] < '0' || host [a] > '9') && host[a] != '.')
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[listen:] host should be ip address"); 
    server.server_listen = port;
    server.server_host = host;
    ++server_vars.num_of_listen;
}
bool ft_check_if_error_pages_numbers_data_valid(s_variables *vars,s_error_page &error_page, s_error_file_variables &error_file_vars)
{
    error_file_vars.num_of_error_page_parm++;
    for(unsigned int a = 0; vars->string[a]; a++)
        if(vars->string[a] < 48 || vars->string[a] > 57)
            return(false);
    if(vars->string == "204")
        ++error_file_vars.num_of_204;
    else if(vars->string == "400")
        ++error_file_vars.num_of_400;
    else if(vars->string == "401")
        ++error_file_vars.num_of_401;
    else if(vars->string == "403")
        ++error_file_vars.num_of_403;
    else if(vars->string == "404")
        ++error_file_vars.num_of_404;
    else if(vars->string == "405")
        ++error_file_vars.num_of_405;
    else if(vars->string == "409")
        ++error_file_vars.num_of_409;
    else if(vars->string == "413")
        ++error_file_vars.num_of_413;
    else if(vars->string == "414")
        ++error_file_vars.num_of_414;
    else if(vars->string == "500")
        ++error_file_vars.num_of_500;
    else if(vars->string == "501")
        ++error_file_vars.num_of_501;
    else if(vars->string == "502")
        ++error_file_vars.num_of_502;
    else if(vars->string == "503")
        ++error_file_vars.num_of_503;
    else if(vars->string == "504")
        ++error_file_vars.num_of_504;
    else
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[error_page:][number] unknwon");
    if(error_file_vars.num_of_504 > 1 || error_file_vars.num_of_503 > 1
        || error_file_vars.num_of_502 > 1 || error_file_vars.num_of_500 > 1
        || error_file_vars.num_of_404 > 1 || error_file_vars.num_of_403 > 1
        || error_file_vars.num_of_401 > 1 || error_file_vars.num_of_204 > 1
        || error_file_vars.num_of_501 > 1 || error_file_vars.num_of_400 > 1
        || error_file_vars.num_of_414 > 1 || error_file_vars.num_of_413 > 1
        || error_file_vars.num_of_405 > 1 || error_file_vars.num_of_409 > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[error_page:][number] duplicated detected");
    if(!error_file_vars.num_of_504 && !error_file_vars.num_of_503 && !error_file_vars.num_of_502 && !error_file_vars.num_of_500
        && !error_file_vars.num_of_404 && !error_file_vars.num_of_403 && 
!error_file_vars.num_of_401 && !error_file_vars.num_of_501
        && !error_file_vars.num_of_409 && !error_file_vars.num_of_204 && !error_file_vars.num_of_405 
        && !error_file_vars.num_of_413 && !error_file_vars.num_of_414 && !error_file_vars.num_of_400)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[error_page:][number] not exist");
    error_page.error_numbers.push_back(vars->string);
    return(true);
}
void ft_put_error_page_data(s_variables *vars, s_server &server,s_error_file_variables &error_file_vars)
{
    std::stringstream sstr(vars->config_file_line);
    s_error_page error_page;

    error_file_vars.num_of_error_page_parm = 0;
    sstr >> vars->string;
    while(sstr >> vars->string && ft_check_if_error_pages_numbers_data_valid(vars,error_page,error_file_vars))
        ;
    error_page.error_file_path = vars->string;
    if(error_page.error_numbers.begin() == error_page.error_numbers.end())
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[error_page:][numbers] data not exist");
    if(error_page.error_file_path == "" || error_file_vars.num_of_error_page_parm == 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[error_page:][error_file_path] data not exist");
    if(error_page.error_file_path[0] != '/')
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[error_page:][error_file_path] it must be absolute path");
    server.error_page.push_back(error_page);
}
void ft_put_client_body_size_data(s_variables *vars, s_server &server)
{
    static int num_of_times_call_funtion = 0;
    static int num_of_server_last_time = 1;
    int num_of_parm = 0;
    std::string cb_body_data;

    if(vars->num_of_server_know > num_of_server_last_time)
    {
        ++num_of_server_last_time;
        num_of_times_call_funtion = 0;
    }
    if(++num_of_times_call_funtion > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[client_body_size:] duplicated detected");
    std::stringstream sstr(vars->config_file_line);

    sstr >> vars->string;
    while(sstr >> cb_body_data)
        ++num_of_parm;
    server.server_cb_size = ft_check_if_cb_body_data_valid(vars,cb_body_data,num_of_parm);
}
void ft_put_server_root_data(s_variables *vars, s_server &server, s_server_variables &server_vars)
{
    static int num_of_times_call_funtion = 0;
    static int num_of_server_last_time = 1;
    int num_of_parm = 0;

    if(vars->num_of_server_know > num_of_server_last_time)
    {
        ++num_of_server_last_time;
        num_of_times_call_funtion = 0;
    }
    if(++num_of_times_call_funtion > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[server_root:] duplicated detected");
    std::stringstream sstr(vars->config_file_line);
    sstr >> vars->string;
    while(sstr >> server.root)
        ++num_of_parm;
    if(num_of_parm > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[server_root:] have more then one parameter");
    if(server.root == "")
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[server_root:] data not exist");
    if(server.root[0]  != '/')
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[root:] it must be absolute path");
    ++server_vars.num_of_root;
}
void ft_put_server_index_data(s_variables *vars, s_server &server, s_server_variables &server_vars)
{
    static int num_of_times_call_funtion = 0;
    static int num_of_server_last_time = 1;

    if(vars->num_of_server_know > num_of_server_last_time)
    {
        ++num_of_server_last_time;
        num_of_times_call_funtion = 0;
    }
    if(++num_of_times_call_funtion > 1)
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[server_index:] duplicated detected");
    std::stringstream sstr(vars->config_file_line);
    std::vector<std::string> index;
    sstr >> vars->string;
    while(sstr >> vars->string)
        index.push_back(vars->string);
    server.index = index;
    if(server.index.begin() == server.index.end())
        err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
            "[server_index:] data not exist");
    ++server_vars.num_of_index;
}
void ft_put_location_data(s_variables *vars,s_location &location, s_location_variables &location_vars)
{
    
    if(vars->string == "root")
    {
        std::stringstream sstr(vars->config_file_line);
        int num_of_parm = 0;
        sstr >> vars->string;
        while(sstr >> location.location_root)
            ++num_of_parm;
        if(num_of_parm > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[root:] have more then one parameter");
        if(location.location_root == "")
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[root:] data not exist");
        if(location.location_root[0] != '/')
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[root:] it must be absolute path");
        ++location_vars.num_of_root;
        if(location_vars.num_of_root > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[root:] duplicated detected");
    }
    else if(vars->string == "index")
    {
        std::stringstream sstr(vars->config_file_line);
        std::vector<std::string> index;
        sstr >> vars->string;
        while(sstr >> vars->string)
            index.push_back(vars->string);
        location.location_index = index;
        if(location.location_index.begin() == location.location_index.end())
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED + "[index:] data not exist");
        ++location_vars.num_of_index;
        if(location_vars.num_of_index > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED + "[index:] duplicated detected");
    }
    else if(vars->string == "accepted_methods")
    {
        std::stringstream sstr(vars->config_file_line);
        std::vector<std::string> methods;
        sstr >> vars->string;
        while(sstr >> vars->string)
        {
            if (vars->string == "GET")
            {
                methods.push_back(vars->string);
                ++location_vars.num_of_get;
            }
            else if (vars->string == "POST")
            {
                methods.push_back(vars->string);
                ++location_vars.num_of_post;
            }
            else if (vars->string == "DELETE")
            {
                methods.push_back(vars->string);
                ++location_vars.num_of_delete;
            }
            else
                err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                    "unknown method");
        }
        ++location_vars.num_of_accepted_methods;
        if(location_vars.num_of_accepted_methods > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[accepted_methods:] duplicated detected");
        else if(location_vars.num_of_get > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[accepted_methods:][GET:] duplicated detected");
        else if(location_vars.num_of_post > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[accepted_methods:][POST:] duplicated detected");
        else if(location_vars.num_of_delete > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[accepted_methods:][DELETE:] duplicated detected");
        if(!location_vars.num_of_get && !location_vars.num_of_post && !location_vars.num_of_delete)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[accepted_methods :] data not exist");
        location.accepted_methods = methods;
    }
    else if(vars->string == "autoindex")
    {
        std::stringstream sstr(vars->config_file_line);
        int num_of_parm = 0;

        sstr >> vars->string;
        while(sstr >> vars->string)
            ++num_of_parm;
        if(num_of_parm > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[autoindex:] have more then one parameter");
        ++location_vars.num_of_autoindex;
        if(location_vars.num_of_autoindex > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[autoindex:] duplicated detected");
        else if(vars->string == "on")
            location.autoindex = true;
        else if (vars->string == "off")
            location.autoindex = false;
        else
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[autoindex:] syntax unknown");
    }
    else if(vars->string == "return")
    {
        std::stringstream sstr(vars->config_file_line);
        int num_of_parm = 0;
        sstr >> vars->string;
        sstr >> vars->string;
        if(vars->string != "301")
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[return:] unknown return number");
        while(sstr >> location.return_url)
            ++num_of_parm;
        if(num_of_parm > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[return:] have more then one parameter");
        ++location_vars.num_of_return;
        if(location_vars.num_of_return > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[return:] duplicated detected");
        else if(!num_of_parm)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[return:] have no data");
    }
    else if(vars->string == "upload_path")
    {
        std::stringstream sstr(vars->config_file_line);
        int num_of_parm = 0;
        sstr >> vars->string;
        while(sstr >> location.upload_path)
            ++num_of_parm;
        if(num_of_parm > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[upload_path:] have more then one parameter");
        ++location_vars.num_of_upload_path;
        if(location_vars.num_of_upload_path > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[upload_path:] duplicated detected");
        else if(!num_of_parm)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[upload_path:] have no data");
        else if(location.upload_path[0] != '/')
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[upload_path:] it must be absolute path");


    }
    else if(vars->string == "cgi_accept")
    {
        std::stringstream sstr(vars->config_file_line);
        std::map<std::string,std::string> cgi;
        sstr >> vars->string;
        while(sstr >> vars->string)
        {
            if (vars->string == ".php")
            {
                cgi["php"] = ".php";
                ++location_vars.num_of_php;
            }
            else if (vars->string == ".py")
            {
                cgi["python"] = ".py";
                ++location_vars.num_of_py;
            }
            else if (vars->string == ".pl")
            {
                cgi["perl"] = ".pl";
                ++location_vars.num_of_pl;
            }
            else
                err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                    "unknown method");
        }
        ++location_vars.num_of_cgi_accept;
        if(location_vars.num_of_cgi_accept > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_accept:] duplicated detected");
        else if(location_vars.num_of_php > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_accept:][.php:] duplicated detected");
        else if(location_vars.num_of_py > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_accept:][.py:] duplicated detected");
        else if(location_vars.num_of_pl > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_accept:][.pl:] duplicated detected");
        if(!location_vars.num_of_php && !location_vars.num_of_py && !location_vars.num_of_pl)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_accept :] data not exist");
        location.cgi_accept = cgi;
    }
    else if(vars->string == "cgi_path")
    {
        std::stringstream sstr(vars->config_file_line);
        int num_of_parm = 0;
        sstr >> vars->string;
        while(sstr >> location.cgi_path)
            ++num_of_parm;
        if(num_of_parm > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_path:] have more then one parameter");
        ++location_vars.num_of_cgi_path;
        if(location_vars.num_of_cgi_path > 1)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_path:] duplicated detected");
        else if(!num_of_parm)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_path:] have no data");
        else if(location.cgi_path[0] != '/')
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[cgi_path:] it must be absolute path");


    }
    else if(vars->string != "")
        if(vars->string[0] != '#')
        err1("child location tag not know !");
}
void ft_check_server_data(s_server_variables &server_vars, s_variables *vars)
{
    if(!server_vars.num_of_listen)
        err1(C_BLUE + vars->config_file_name + "::Server"+ std::to_string(vars->num_of_server_know) + C_PURPLE + " -->\t"+ C_RED +
            "[listen:] not found");
}

void ft_check_location_data(s_location &location, s_server const &server, s_server_variables &server_vars, s_location_variables &location_variables, s_variables *vars)
{
    if(!location_variables.num_of_accepted_methods)
        err1(C_BLUE + vars->config_file_name + "::Server"+ std::to_string(vars->num_of_server_know)+ "::Location" + std::to_string(server_vars.num_of_locations) + C_PURPLE + " -->\t"+ C_RED +
            "[accepted_methods:] not found");
    if(!server_vars.num_of_root && !location_variables.num_of_root)
        err1(C_BLUE + vars->config_file_name + "::Server"+ std::to_string(vars->num_of_server_know)+ "::Location" + std::to_string(server_vars.num_of_locations) + C_PURPLE + " -->\t"+ C_RED +
            "[server_root:] not found");
    else if(!server_vars.num_of_index && !location_variables.num_of_index)
        err1(C_BLUE + vars->config_file_name + "::Server"+ std::to_string(vars->num_of_server_know)+ "::Location" + std::to_string(server_vars.num_of_locations) + C_PURPLE + " -->\t"+ C_RED +
            "[server_index:] not found");
    else if(!location_variables.num_of_root)
        location.location_root =  server.root;
    else if(!location_variables.num_of_index)
        location.location_index =  server.index;
}

void ft_next_config_file(s_variables *vars)
{
    unsigned int new_server_spaces;
    unsigned int new_location_spaces;
    
    vars->num_of_line_know++;
    // TODO: read
    std::getline(vars->config_file,vars->config_file_line);
    vars->string.clear();
    std::stringstream strstream(vars->config_file_line);
    strstream >> vars->string;
    if(vars->string == "server:")
        vars->server_line_spaces = ft_calc_num_of_spaces_in_line(vars->config_file_line);
    else if(vars->string == "location:")
        vars->location_line_spaces = ft_calc_num_of_spaces_in_line(vars->config_file_line);
    if(vars->string == "index" || vars->string == "root"
        || vars->string == "accepted_methods" || vars->string == "autoindex"
        || vars->string == "return" || vars->string == "upload_path"
        || vars->string == "cgi_accept" || vars->string == "cgi_path")
    {
        new_location_spaces = ft_calc_num_of_spaces_in_line(vars->config_file_line);
        if(new_location_spaces <= vars->location_line_spaces && vars->location_line_spaces != MAX_LINE)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "no new tab after location tag");
        else if(new_location_spaces != vars->after_location_line_spaces && vars->after_location_line_spaces != MAX_LINE)
            err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "no match line after location tag");
        vars->after_location_line_spaces = new_location_spaces;
    }
    else if(vars->string != "" && vars->string != "server:")
    {
        if(vars->string[0] != '#')
        {
            new_server_spaces = ft_calc_num_of_spaces_in_line(vars->config_file_line);
            if(vars->server_line_spaces == MAX_LINE)
                err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                    " outside server tag");
            else if(new_server_spaces <= vars->server_line_spaces && vars->server_line_spaces != MAX_LINE)
                err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                    "no new tab after server tag");
            else if(new_server_spaces != vars->after_server_line_spaces && vars->after_server_line_spaces != MAX_LINE)
                err1(C_BLUE + vars->config_file_name + "::" + std::to_string(vars->num_of_line_know) + C_PURPLE + " -->\t"+ C_RED + 
                    "no match line after server tag");
            vars->after_server_line_spaces = new_server_spaces;
        }
    }
}
void ft_add_location_root_index(s_location &location)
{
    for(std::vector<std::string>::iterator it = location.location_index.begin() ; it < location.location_index.end() ; it++)
        location.location_root_index.push_back(location.location_root + (*it));
}

void s_variables_init(s_variables &vars)
{
    vars.after_location_line_spaces = MAX_LINE;
    vars.after_server_line_spaces = MAX_LINE;
    vars.server_line_spaces = MAX_LINE;
    vars.location_line_spaces = MAX_LINE;
    vars.num_of_server_know = 0;
    vars.num_of_line_know = 0;
}

void s_server_variables_init(s_server_variables &server_vars)
{
    server_vars.num_of_listen = 0;
    server_vars.num_of_root = 0;
    server_vars.num_of_index = 0;
    server_vars.num_of_locations = 0;
}

void s_error_file_variables_init(s_error_file_variables &error_file_variables)
{
    error_file_variables.num_of_401 = 0;
    error_file_variables.num_of_403 = 0;
    error_file_variables.num_of_404 = 0;
    error_file_variables.num_of_500 = 0;
    error_file_variables.num_of_502 = 0;
    error_file_variables.num_of_503 = 0;
    error_file_variables.num_of_504 = 0;
    error_file_variables.num_of_204 = 0;
    error_file_variables.num_of_400 = 0;
    error_file_variables.num_of_405 = 0;
    error_file_variables.num_of_409 = 0;
    error_file_variables.num_of_413 = 0;
    error_file_variables.num_of_414 = 0;
    error_file_variables.num_of_501 = 0;
}

void s_location_variables_init(s_location_variables &location_variables)
{
    location_variables.num_of_root = 0;
    location_variables.num_of_index = 0;
    location_variables.num_of_accepted_methods = 0;
    location_variables.num_of_cgi_accept = 0;
    location_variables.num_of_get = 0;
    location_variables.num_of_post = 0;
    location_variables.num_of_delete = 0;
    location_variables.num_of_php = 0;
    location_variables.num_of_py = 0;
    location_variables.num_of_pl = 0;
    location_variables.num_of_upload_path = 0;
    location_variables.num_of_cgi_path = 0;
    location_variables.num_of_autoindex = 0;
    location_variables.num_of_return = 0;
}
