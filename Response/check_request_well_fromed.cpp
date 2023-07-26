#include "Response.hpp"

bool Response::check_request(tRequest &req)
{
    
    if (req.request.bad_request == NOT_IMPLEMNT)
    {
        this->status = "501";
        this->gen_Response.id = req.id;
        this->override_error_page();
        return (false);

    }
    else if (req.request.bad_request == ENTITY_TOO_LONG)
    {
        this->status = "414";
        this->gen_Response.id = req.id;
        this->override_error_page();
        return (false);
    }
    else if (req.request.bad_request == BAD_REQ)
    {
        this->status = "400";
        this->gen_Response.id = req.id;
        this->override_error_page();
        return (false);
    }
    else if (req.request.bad_request == METHOD_UNK)
    {
        this->status = "405";
        this->gen_Response.id = req.id;
        this->override_error_page();

        return (false);
    }
    else if (req.request.bad_request == ENTITY_TO_LARG)
    {
        this->status = "413";
        this->gen_Response.id = req.id;
        this->override_error_page();
    }
    return (true);
}
bool Response::allowed_character(std::string str)
{
    std::size_t found = str.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=");

   if (found!=std::string::npos)
      return (false);
    return (true);
}

bool Response::check_request_well_fromed()
{
    if (!this->headers["Transfer-Encoding"].empty() && this->headers["Transfer-Encoding"] != "chunked")
    {
        this->status = "501";
        this->override_error_page();
    }
    else if (this->headers["method"] == "POST" && this->headers["Transfer-Encoding"].empty() && !this->is_content_length )
    {
        this->status = "400";
        this->override_error_page();
    }
    else if (!allowed_character(this->headers["uri"]))
    {
        this->status = "400";
        this->override_error_page();
    }
    else if (this->headers["uri"].length() > 2048)
    {
        this->status = "414";
        this->override_error_page();
    }
    else if (this->headers["method"] == "POST" && this->content_length > this->max_body_size)
    {
        this->status = "413";
        this->override_error_page();
    }
    else
    {
        return (true);
    }
    return (false);
}