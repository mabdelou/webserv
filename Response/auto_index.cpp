# include "Response.hpp"

std::string Response::gen_body_auto_inex(void)
{
    std::string res;
    std::string path;
    std::vector<std::string>::iterator it;
    std::string root;
    root = this->location.location_root;
    if ( root[root.length() - 1] != '/')
        root += "/";
    res= "<!DOCTYPE HTML><html><body><h1>Index Of :</h1><hr>";
    for (it = this->auto_index_files.begin() ; it < this->auto_index_files.end() ; it++)
    {
        if ((*it) == "../" && this->path_uri == root)
        {
            continue ;
        }
        if ((*it)[(*it).length() - 1] == '/')
        {
            path = (*it);
            path.erase(path.length() - 1);
        }
        else
            path = (*it);
        res+= "<a href=\"" + (*it) + "\">" + path + "</a><br>";
    }
    res+="<hr></body></html>";
    this->content_length = res.length();
    this->headers["Content-type"] = "text/html";
    (this->gen_Response).headers = this->gen_header("200");
    this->auto_index_files.clear();
    return res;
}

void    Response::auto_index(void)
{
    struct dirent *de;
    std::string res;
    DIR *dr = opendir(this->path_uri.c_str());
    if (dr) 
    {
        while ((de = readdir(dr)) != NULL)
        {
            if (!strcmp(de->d_name,".") || !strcmp(de->d_name,".git"))
                continue ;
            if (de->d_type == DT_DIR)
            {
                res = de->d_name;
                res +="/";
                this->auto_index_files.push_back(res);
            }
            else
                this->auto_index_files.push_back(de->d_name);
        }
        closedir(dr);    
        (this->gen_Response).str_err = this->gen_body_auto_inex();
    }else
    {
        this->gen_Response.status = ERR__;
        this->status = "404";
        this->override_error_page();
    }
}