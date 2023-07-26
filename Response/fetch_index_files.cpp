# include "Response.hpp"

bool Response::check_cgi(std::string path_file)
{
    if (extention(path_file) == "php" && !this->accept_cgi["php"].empty())
        return true ;
    if (extention(path_file) == "py" && !this->accept_cgi["python"].empty())
        return true ;
    return false;
}

void    Response::set_response_file(std::string path_file, struct stat sb)
{
    if (this->headers["method"] == "DELETE")
    {
        std::string rm ;
        if (S_ISREG(sb.st_mode) && !((sb.st_mode & S_IRUSR) && (sb.st_mode & S_IWUSR)))
        {
            this->status = "403"; 
            this->override_error_page();
            return ;
        }
        rm = "rm -r " + path_file + " 2> /dev/null";
        if (system(rm.c_str()) != 0)
        {
            this->status = "500"; 
            this->override_error_page();
            return ;
        }
        this->gen_Response.status = ERR__;
        this->status = "204";
        this->override_error_page();
        return ;
    }
    if (this->headers["method"] == "POST" && !this->cgi)
    {
        this->gen_Response.status = ERR__;
        this->status = "403";
        this->override_error_page();
        return ;
    }
    if (!this->cgi || !check_cgi(path_file))
    {
        if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IRUSR))
        {
            this->status = "200";
            this->gen_Response.status = TEXT__;
            this->headers["Content-type"] = this->content_types[this->extention(path_file)];
            this->content_length = sb.st_size;
            (this->gen_Response).headers = this->gen_header("200");
            (this->gen_Response).path = path_file;
        }
        else
        {
            this->gen_Response.status = ERR__;
            this->status = "404";
            this->override_error_page();

        }
    }
    else
    {
        this->cgi_exec(path_file);
    }

}

void Response::fetch_index_files(struct stat sb)
{
    std::string root;
    std::vector<std::string>::iterator it;
    root = this->location.location_root;
    if(root[root.length() - 1 ] != '/')
        root += "/";
    for (it = location.location_index.begin(); it != location.location_index.end(); it++)
    {
        if (stat((root + (*it)).c_str(), &sb) == 0)
        {
            if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IRUSR))
            {
                this->content_length = sb.st_size;
                this->set_response_file(root + (*it), sb);     // file || CGI
                break ;
            }else
            {
                this->gen_Response.status = ERR__;
                this->status = "404";
                this->override_error_page();
                break ;
            }
        }
    }
    if ( it == location.location_index.end())
    {
        if (!this->location.autoindex  || this->headers["method"] == "DELETE")
        {
            this->gen_Response.status = ERR__;
            this->status = "403";
            this->override_error_page();
        }
        else
        {
            this->gen_Response.status = ERR__;
            this->status = "200";
            this->auto_index();   // this->autoindex();
        }
    }
}