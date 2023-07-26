#include "Response.hpp"
extern char                     **environ;

void Response::cgi_exec(std::string path_file)
{
    int pid;
    int fds[2];
    int fds_body[2];
    std::string cgi_path;

    if (!this->path_cgi.empty() && !this->accept_cgi["php"].empty() && extention(path_file) == "php")
    {
        cgi_path = this->path_cgi;
    }
    else if (!this->accept_cgi["python"].empty())
    {
        cgi_path = "/usr/bin/python";
    }
        if(pipe(fds) == -1)
        {
            this->status = "500"; 
            this->override_error_page();
            return ;
        }
        pid  = fork();
        if(pid == -1)
        {
            this->status = "500"; 
            this->override_error_page();
            return ;
        }
        if(pid == 0)
        {
            if (this->headers["method"] == "POST")
            {
                if(pipe(fds_body) == -1)
                    exit(3);
                write(fds_body[1],this->query_body.c_str(),query_body.length());
                dup2(fds_body[0],0);
                close(fds_body[0]);
                close(fds_body[1]);
                setenv("CONTENT_LENGTH",std::to_string(this->content_length).c_str(),1);
                setenv("CONTENT_TYPE",this->content_type.c_str(),1);
            }
            dup2(fds[1],1);
            close(fds[0]);
            close(fds[1]);
            char *args[3];
            args[0] = (char *)cgi_path.c_str();
            args[1] = (char *)path_file.c_str();
            args[2] = NULL;
            setenv("GATEWAY_INTERFACE","CGI/1.1",1);
            setenv("SERVER_PROTOCOL","HTTP/1.1",1);
            setenv("REQUEST_METHOD",this->headers["method"].c_str(),1);
            setenv("REDIRECT_STATUS","200",1);
            setenv("PATH_INFO",this->query_path.c_str(),1);
            setenv("SCRIPT_FILENAME",path_file.c_str(),1);
            setenv("QUERY_STRING",this->query_str.c_str(),1);
            if(execve(args[0],args,environ) == -1)
                exit(3);
            
        }

        this->status = "200";
        this->gen_Response.status = CGI__;
        this->gen_Response.pid = pid;
        (this->gen_Response).headers  = "HTTP/1.1 ";
        (this->gen_Response).headers  +=  status + " " ;
        (this->gen_Response).headers  += this->status_str[status] + "\r\n";
        (this->gen_Response).fileFd = fds[0];
    close(fds[1]);
}