#include "Response.hpp"


std::string Response::error_page(void) {

    std::string html;

    html = "<!DOCTYPE html> <html lang=\"en\"><head><meta charset=\"UTF-8\"><title>" + this->status + " " + this->status_str[this->status]+"</title><style>h1{text-align: center;padding-top: 100px;}p{text-align: center;}</style></head>\
    <body><h1>" + this->status + " " + this->status_str[this->status] + "</h1><br><hr><p>MMA webserv</p></body></html>";
    this->content_length  = html.length();
    return html;
}

void    Response::override_error_page()
{
    struct stat sb;
    if (stat((*this->server).map_error_pages[this->status].c_str(), &sb) == 0)
    {
        if (S_ISREG(sb.st_mode) && (sb.st_mode & S_IRUSR))
        {
            this->gen_Response.status = TEXT__;
            (this->gen_Response).path = (*this->server).map_error_pages[this->status];
            this->headers["Content-type"] = this->content_types[this->extention((*this->server).map_error_pages[this->status])];
            this->content_length = sb.st_size ;
        }
    }
    else
    {
        this->gen_Response.status = ERR__;
        (this->gen_Response).str_err = this->error_page();
        this->headers["Content-type"] = "text/html";
    }
    (this->gen_Response).headers = gen_header(this->status);
    return ;
}
