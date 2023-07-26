#include "Utilities.hpp"

// #include "../socket/utils.hpp"

int parssing_part(int argc, char **argv, std::vector<s_server> &servers_list)
{
    s_variables vars;

    s_variables_init(vars);
    //open configuration file
    // TODO: open 
    // some codition for protect programme
    if(argc != 2)
        err1("bad_of_argumment!");
    vars.config_file.open(argv[1], std::ifstream::in);
    if (!ft_check_config_file_name(argv[1]))
        err1("bad config_file_name!");

    else if (!vars.config_file.is_open())
        err1("file not exist!");
    vars.config_file_name = argv[1];
    // the while loop for put configfile data
    while (vars.config_file.good())
    {
        s_server server;
        s_server_variables server_vars;
        s_error_file_variables error_file_variables;
        s_server_variables_init(server_vars);
        s_error_file_variables_init(error_file_variables);
        vars.after_server_line_spaces = MAX_LINE;
        if(vars.string == "server:")
        {
            ++vars.num_of_server_know;
            ft_next_config_file(&vars);
            while(vars.string != "server:" && vars.config_file.good())
            {
                if(vars.string == "listen")
                    ft_put_server_listen_data(&vars, server, server_vars,servers_list);
                else if(vars.string == "error_page")
                    ft_put_error_page_data(&vars, server, error_file_variables);
                else if(vars.string == "client_body_size")
                    ft_put_client_body_size_data(&vars, server);
                else if(vars.string == "server_root")
                    ft_put_server_root_data(&vars, server, server_vars);
                else if(vars.string == "server_index")
                    ft_put_server_index_data(&vars, server, server_vars);
                else if(vars.string == "location:")
                {
                    s_location location;
                    s_location_variables location_variables;
                    location.autoindex = false;
                    location.upload_path = "";
                    s_location_variables_init(location_variables);
                    server_vars.num_of_locations++;
                    vars.after_location_line_spaces = MAX_LINE;
                    std::stringstream sstr1(vars.config_file_line);
                    sstr1 >> vars.string;
                    sstr1 >> location.client_location;
                    if(location.client_location == "")
                        err1(C_BLUE + vars.config_file_name + "::" + std::to_string(vars.num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                            "[client_location :] data not exist");
                    else if(sstr1 >> vars.string)
                        err1(C_BLUE + vars.config_file_name + "::" + std::to_string(vars.num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                            "[client_location :] have more then one parameter");
                    else if(location.client_location[0] != '/')
                        err1(C_BLUE + vars.config_file_name + "::" + std::to_string(vars.num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                            "[client_location :] should be start with '/'");
                    vars.string = location.client_location;
                    if(vars.string != "location:")
                        ft_next_config_file(&vars);
                    while(vars.string != "location:" && vars.string != "server:" && vars.config_file.good())
                    {
                        ft_put_location_data(&vars, location, location_variables);
                        ft_next_config_file(&vars);
                    }
                    ft_check_location_data(location, server, server_vars, location_variables,&vars);
                    ft_add_location_root_index(location);
                    server.locations.push_back(location);
                }
                else if(vars.string != "")
                    if(vars.string[0] != '#')
                        err1(C_BLUE + vars.config_file_name + "::" + std::to_string(vars.num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                            "child server tag not know !");
                if(vars.string != "location:" && vars.string != "server:")
                    ft_next_config_file(&vars);
            }
            ft_check_server_data(server_vars, &vars);

            std::vector<s_error_page>::iterator it;
            std::vector<std::string>::iterator it1;
            for(it = server.error_page.begin(); it < server.error_page.end(); it++)
            {
                for (it1 = (*it).error_numbers.begin(); it1 < (*it).error_numbers.end(); it1++)
                {
                    server.map_error_pages[(*it1)] = (*it).error_file_path;
                }
            }
            servers_list.push_back(server);
        }
        if(vars.string != "server:")
            ft_next_config_file(&vars);
    }
    if(servers_list.begin() == servers_list.end())
            err1(C_BLUE + vars.config_file_name + "::" + std::to_string(vars.num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[server:] tag not detected");
    for (std::vector<s_server>::iterator it = servers_list.begin() ; it < servers_list.end() ; it++)
        if((*it).locations.begin() == (*it).locations.end())
            err1(C_BLUE + vars.config_file_name + "::" + std::to_string(vars.num_of_line_know) + C_PURPLE + " -->\t"+ C_RED +
                "[location:] tag not detected");
    
    
    return(0);
}