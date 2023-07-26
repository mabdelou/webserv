#ifndef RESPONSE_H
# define RESPONSE_H
# include <iostream>
# include <fstream>
# include <sstream>
# include <ctime>
# include <map>
# include <fcntl.h>
# include <list>
# include "../parssing/parssing/Parssing.hpp"
# include "../parssing/request_parssing/parssing_files/Request.hpp"
# include <unistd.h>
# include <sys/stat.h>
# include <dirent.h>
# include <vector>



class Response
{
    private:
		tResponse							gen_Response;
        std::string                         path_cgi;
        std::string                         content_type;
        std::map<std::string, std::string>  accept_cgi;
        std::vector<s_server>::iterator		server;
        std::map<std::string, std::string>  content_types;         
		std::map<std::string, std::string>	status_str;
        std::map<std::string, std::string>	headers;
        std::vector<std::string>            auto_index_files;
		std::string							status;
        std::string                         path_uri;
        t_location                          location;
        size_t                              max_body_size;
        bool                                cgi;
        size_t                              content_length;
        bool                                is_content_length;
        std::string                         query_str;
        std::string                         query_path;
        std::string                         query_body;
        bool                                check_request_well_fromed();
        bool                                allowed_character(std::string str);
		tResponse                           generate_reponse(std::string status);
		std::string                         gen_header(std::string status);
        void                                generate_reponses_error(void);
        bool                                Method_Not_allowded();
        void                                override_error_page();
        std::string                         error_page(void);
        void                                set_response_file(std::string path_file,struct stat sb);
        void                                auto_index(void);
        std::string                         gen_body_auto_inex(void);
        bool                                get_location(t_server server, std::string uri);
        void                                return_301(std::string str);
        void                                fetch_index_files(struct stat sb);
        bool                                Is_Dir(struct stat sb);
        void                                get_requested_resource();                                
        std::string                         extention(std::string path);
        void                                upload();
        void                                clearAll( void );
		void                                generate_reponses(tRequest &req);
        bool                                delete_all_Contents();
        void                                cgi_exec(std::string path_file);
        bool                                check_cgi(std::string path_file);
        bool                                check_request(tRequest &req);
    public:
        Response();
        virtual ~Response();
		void	Responses(void);


};

void    handle_Response();
#endif
