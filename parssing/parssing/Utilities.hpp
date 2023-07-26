#ifndef UTILITIES_H__
# define  UTILITIES_H__

# include "Parssing.hpp"

typedef struct t_variables
{
    std::ifstream           config_file;
    std::string             config_file_name;
    std::string             config_file_line;
    std::string             string;
    unsigned int            server_line_spaces;
    unsigned int            after_server_line_spaces;
    unsigned int            location_line_spaces;
    unsigned int            after_location_line_spaces;
    int                     num_of_server_know;
    int                     num_of_line_know;

}s_variables;

typedef struct t_server_variables
{
    int                         num_of_listen;
    int                         num_of_root;
    int                         num_of_index;
    int                         num_of_locations;

}s_server_variables;

typedef struct t_location_variables
{
    int                         num_of_root;
    int                         num_of_index;
    int                         num_of_accepted_methods;
    int                         num_of_cgi_accept;
    int                         num_of_get;
    int                         num_of_post;
    int                         num_of_delete;
    int                         num_of_php;
    int                         num_of_py;
    int                         num_of_pl;
    int                         num_of_autoindex;
    int                         num_of_return;
    int                         num_of_upload_path;
    int                         num_of_cgi_path;

}s_location_variables;

typedef struct t_error_file_variables
{
    int                         num_of_401;
    int                         num_of_403;
    int                         num_of_404;
    int                         num_of_500;
    int                         num_of_502;
    int                         num_of_503;
    int                         num_of_504;
    int                         num_of_204;
    int                         num_of_400;
    int                         num_of_405;
    int                         num_of_409;
    int                         num_of_413;
    int                         num_of_414;
    int                         num_of_501;
    int                         num_of_error_page_parm;

}s_error_file_variables;

template <class T>
size_t ft_strlen(T config_file_name)
{
    unsigned int a = 0;
    while( config_file_name[++a])
        ;
    return(a);
}
unsigned int    ft_calc_num_of_spaces_in_line(std::string const &str);
uint32_t        ft_check_if_cb_body_data_valid(s_variables *vars, std::string const &data, int num_of_parm);
uint16_t        ft_check_if_listen_data_valid(s_variables *vars, std::string const &data);
bool            ft_check_config_file_name(char *config_file_name);
bool            ft_check_if_error_pages_numbers_data_valid(s_variables *vars,s_error_page &error_page, s_error_file_variables &error_file_vars);
void            ft_display_error(std::vector<s_server>::iterator &it,const std::string &str);
void            ft_put_server_listen_data(s_variables *vars, s_server &server, s_server_variables &server_vars,std::vector<s_server> &servers_list);
void            ft_put_error_page_data(s_variables *vars, s_server &server,s_error_file_variables &error_file_vars);
void            ft_put_client_body_size_data(s_variables *vars, s_server &server);
void            ft_put_server_root_data(s_variables *vars, s_server &server, s_server_variables &server_vars);
void            ft_put_server_index_data(s_variables *vars, s_server &server, s_server_variables &server_vars);
void            ft_put_location_data(s_variables *vars,s_location &location, s_location_variables &location_vars);
void            ft_check_server_data(s_server_variables &server_vars, s_variables *vars);
void            ft_check_location_data(s_location &location, s_server const &server, s_server_variables &server_vars, s_location_variables &location_variables, s_variables *vars);
void            ft_next_config_file(s_variables *vars);
void            ft_add_location_root_index(s_location &location);
void            s_variables_init(s_variables &vars);
void            s_server_variables_init(s_server_variables &server_vars);
void            s_error_file_variables_init(s_error_file_variables &error_file_variables);
void            s_location_variables_init(s_location_variables &location_variables);
void            err1(std::string const &str);
#endif