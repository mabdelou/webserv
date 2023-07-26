#ifndef REQUEST_H__
# define REQUEST_H__

# include <iostream>
# include <fstream>
# include <istream>
# include <sstream>
# include <string>
# include <vector>
# include <list>
# include <map>

typedef struct t_request_body
{
    std::string                 content_disposition;
    std::string                 name;
    std::string                 filename;
    std::string                 content_type;

}s_request_body;

typedef struct t_request_head
{
    int                                    bad_request;
    std::string                             method;
    std::string                             path;
    std::string                             query_data;
    std::string                             query_path;
    std::string                             query_body;
    std::string                             protocol;
    std::string                             host;
    uint16_t                                port;
    std::string                             connection;
    uint32_t                                content_lenght;
    bool                                    is_content_length;
    int32_t                                 num_of_body_data_writed;
    std::string                             transfer_encoding;
    std::string                             boundary;
    std::string                             origin;
    std::string                             referer;
    std::string                             upgrade_insecure_request;
    std::string                             sec_fetch_dest;
    std::string                             sec_fetch_mode;
    std::string                             sec_fetch_site;
    std::string                             sec_fetch_user;
    std::vector<std::string>                accept;
    std::vector<std::string>                accept_language;
    std::vector<std::string>                accept_encoding;
    s_request_body                          request_body;

}s_request_head;

# include "../../../shared/shared.hpp"

void request_parssing(void);

#endif
