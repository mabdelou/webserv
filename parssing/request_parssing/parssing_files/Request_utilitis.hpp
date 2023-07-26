#ifndef REQUEST_UTILITIS_H__
# define REQUEST_UTILITIS_H__

# include <iostream>
# include <fstream>
# include <istream>
# include <sstream>
# include <string>
# include <vector>
# include <list>
# include <map>
# include "Request.hpp"
#include "../../parssing/Parssing.hpp"

// extern std::list<gFileInfo> gfileclient;
// extern std::vector<tRequest> gRequest;

typedef struct t_request_data
{
    std::ofstream       *out_file;
    s_request_head      req_head;
    uint32_t            length;
    uint32_t            num_of_bdw;
    bool                can_write;

}s_request_data;

typedef struct t_boundray_data
{
    uint32_t bondray_index;
    std::string old_data;
    bool old_return;

}s_boundray_data;

void    extention_map_init(std::map<std::string,std::string> &content_types);
void    ft_init_map_data(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it);
void    ft_init_data(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it);
bool    check_if_begin_bondray(std::string begin_boundry, std::list<gFileInfo>::iterator &it ,int index);
bool    check_if_end_bondray(std::string end_bondray, std::list<gFileInfo>::iterator &it ,int index);
bool    get_method_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
bool    get_path_and_protocol_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
bool    get_host_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    get_accept_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    get_accept_language_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    get_content_type_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    get_accept_encoding_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    get_connection_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
bool    get_trasfer_encoding_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    get_another_data(std::map<int,s_request_data> &data, std::stringstream &sstream,std::string &str, std::list<gFileInfo>::iterator &it);
bool    get_content_length_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    get_boundray_files_data(std::map<int,s_request_data> &data, std::stringstream &sstream, std::list<gFileInfo>::iterator &it);
void    end_request_parssing(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it);
bool    write_boundray_body_data_v1(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it);
bool    write_chuncked_body_data_v1(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it,std::map<std::string,std::string> &content_types);
bool    write_chuncked_body_data_v2(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it);
bool    write_boundray_body_data_v2(std::map<int,s_request_data> &data, std::list<gFileInfo>::iterator &it);

#endif
