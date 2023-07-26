#include "Response.hpp"

void Response::clearAll( void ) {

    this->gen_Response.id = 0;     
    this->gen_Response.status = 0; 
    this->cgi= false;
    this->path_cgi.clear();
    this->gen_Response.headers.clear();
    this->gen_Response.str_err.clear();
    this->gen_Response.path.clear();   
    this->gen_Response.cgi = 0;    
    this->max_body_size = 0;
    this->content_length = 0;
    this->headers.clear();
    this->auto_index_files.clear();
    this->status.clear();
    this->path_uri.clear();
     this->content_type.clear();

}


Response::Response()
{
    // status_code string 
    this->status_str["200"] = "OK";
    this->status_str["501"] = "Not Implemented";
    this->status_str["404"] = "Not Found";
    this->status_str["400"] = "Bad Request";
    this->status_str["414"] = "Request-URI Too Long";
    this->status_str["413"] = "Request Entity Too Large";
    this->status_str["301"] = "Moved Permanently";
    this->status_str["405"] = "Method Not Allowed";
    this->status_str["403"] = "Forbidden";
    this->status_str["201"] = "Created";
    this->status_str["204"] = "No Content";
    this->status_str["409"] = "Conflict";
    this->status_str["500"] = "Internal Server Error";
    // Content_type
    this->content_types["html"] = "text/html";
	this->content_types["htm"] = "text/html";
	this->content_types["shtml"] = "text/html";
	this->content_types["css"] = "text/css";
	this->content_types["xml"] = "text/css";
	this->content_types["gif"]= "image/gif";
	this->content_types["jpeg"]= "image/jpeg";
	this->content_types["jpg"]= "image/jpeg";
	this->content_types["png"]= "image/png";
	this->content_types["js"]= "application/javascript";
	this->content_types["atom"]= "application/atom+xml";
	this->content_types["rss"]= "application/rss+xml";
	this->content_types["mml"]= "text/mathml";
	this->content_types["txt"]= "text/plain";
	this->content_types["jad"]= "text/vnd.sun.j2me.app-descriptor";
	this->content_types["wml"]= "text/vnd.wap.wml";
	this->content_types["htc"]= "text/x-component";
	this->content_types["png"]= "image/png";
	this->content_types["tif"]= "image/tiff";
	this->content_types["tiff"]= "image/tiff";
	this->content_types["wbmp"]= "image/vnd.wap.wbmp";
	this->content_types["ico"]= "image/x-icon";
	this->content_types["jng"]= "image/x-jng";
	this->content_types["bmp"]= "image/x-ms-bmp";
	this->content_types["svg"]= "image/svg+xml";
	this->content_types["svgz"]= "image/svg+xml";
	this->content_types["webp"]= "image/webp";
	this->content_types["woff"] = "application/font-woff";
	this->content_types["jar"] = "application/java-archive";
	this->content_types["war"] = "application/java-archive";
	this->content_types["ear"] = "application/java-archive";
	this->content_types["json"] = "application/json";
	this->content_types["hqx"] = "application/mac-binhex40";
	this->content_types["doc"] = "application/msword";
	this->content_types["pdf"] = "application/pdf";
	this->content_types["ps"] = "application/postscript";
	this->content_types["eps"] = "application/postscript";
	this->content_types["ai"] = "application/postscript";
	this->content_types["rtf"] = "application/rtf";
	this->content_types["m3u8"] = "application/vnd.apple.mpegurl";
	this->content_types["xls"] = "application/vnd.ms-excel";
	this->content_types["eot"] = "application/vnd.ms-fontobject";
	this->content_types["ppt"] = "application/vnd.ms-powerpoint";
	this->content_types["wmlc"] = "application/vnd.wap.wmlc";
	this->content_types["kml"] = "application/vnd.google-earth.kml+xml";
	this->content_types["kmz"] = "application/vnd.google-earth.kmz";
	this->content_types["7z"] = "application/x-7z-compressed";
	this->content_types["cco"] = "application/x-cocoa";
	this->content_types["jardiff"] = "application/x-java-archive-diff";
	this->content_types["jnlp"] = "application/x-java-jnlp-file";
	this->content_types["run"] = "application/x-makeself";
	this->content_types["pl"] = "application/x-perl";
	this->content_types["pm"] = "application/x-perl";
	this->content_types["prc"] = "application/x-pilot";
	this->content_types["pdb"] = "application/x-pilot";
	this->content_types["rar"] = "application/x-rar-compressed";
	this->content_types["rpm"] = "application/x-redhat-package-manager";
	this->content_types["sea"] = "application/x-sea";
	this->content_types["swf"] = "application/x-shockwave-flash";
	this->content_types["sit"] = "application/x-stuffit";
	this->content_types["tcl"] = "application/x-tcl";
	this->content_types["tk"] = "application/x-tcl";
	this->content_types["der"] = "application/x-x509-ca-cert";
	this->content_types["pem"] = "application/x-x509-ca-cert";
	this->content_types["cert"] = "application/x-x509-ca-cert";
	this->content_types["xpi"] = "application/x-xpinstall";
	this->content_types["xhtml"] = "application/xhtml+xml";
	this->content_types["xspf"] = "application/xspf+xml";
	this->content_types["zip"] = "application/zip";
	this->content_types["bin"] = "application/octet-stream";
	this->content_types["exe"] = "application/octet-stream";
	this->content_types["dll"] = "application/octet-stream";
	this->content_types["deb"] = "application/octet-stream";
	this->content_types["dmg"] = "application/octet-stream";
	this->content_types["iso"] = "application/octet-stream";
	this->content_types["img"] = "application/octet-stream";
	this->content_types["msi"] = "application/octet-stream";
	this->content_types["msp"] = "application/octet-stream";
	this->content_types["msm"] = "application/octet-stream";
	this->content_types["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	this->content_types["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	this->content_types["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	this->content_types["mid"] = "audio/midi";
	this->content_types["midi"] = "audio/midi";
	this->content_types["kar"] = "audio/midi";
	this->content_types["mp3"] = "audio/mpeg";
	this->content_types["ogg"] = "audio/ogg";
	this->content_types["m4a"] = "audio/x-m4a";
	this->content_types["ra"] = "audio/x-realaudio";
	this->content_types["3gpp"] = "video/3gpp";
	this->content_types["3gp"] = "video/3gpp";
	this->content_types["ts"] = "video/mp2t";
	this->content_types["mp4"] = "video/mp4";
	this->content_types["mpeg"] = "video/mpeg";
	this->content_types["mpg"] = "video/mpeg";
	this->content_types["mov"] = "video/quicktime";
	this->content_types["webm"] = "video/webm";
	this->content_types["flv"] = "video/x-flv";
	this->content_types["m4v"] = "video/x-m4v";
	this->content_types["mng"] = "video/x-mng";
	this->content_types["asx"] = "video/x-ms-asf";
	this->content_types["asf"] = "video/x-ms-asf";
	this->content_types["wmv"] = "video/x-ms-wmv";
	this->content_types["avi"] = "video/x-msvideo";
    this->content_types["php"] = "application/x-www-form-urlencoded";  
}

std::string  Response::extention(std::string path)
{
    int position=path.find_last_of(".");
    std::string res = path.substr(position+1);
    return (res);
}

std::string Response::gen_header(std::string status)
{
    time_t now = time(0);
    char* dt = ctime(&now);
    std::string time = dt;
    time.erase(time.length() - 1);
    std::string res;
    std::stringstream ss;
    std::string str;
    ss << this->content_length;
    ss >> str;
    res = "HTTP/1.1 ";
    res +=  status + " " ;
    res += this->status_str[status] + "\r\n";
    res += "Date: " +  time + "\r\nServer: MMA \r\n";
    res += "Content-Length: " + str + "\r\n";    // Content-Type: text/html
    res += "Content-Type: " + this->headers["Content-type"] + "\r\n";// Content-Length: 88
    res += "Connection: Closed \r\n\r\n";   // Connection: Closed";
    return (res);
}

bool    Response::Method_Not_allowded()
{
    std::vector<std::string>::iterator         ptr;

    for (ptr = this->location.accepted_methods.begin(); ptr < this->location.accepted_methods.end(); ptr++)
    {
        if (this->headers["method"] == (*ptr))
            return (false);
    }
    this->status = "405";
    this->override_error_page();
    return (true);
}

bool        Response::get_location(t_server server, std::string uri)
{
    std::vector<s_location>::iterator         ptr;
    std::vector<s_location>::iterator         slash_location;
    std::string                               cpy_uri;
    std::string                               tmp;
    std::string                               tmp1;
    bool                                      slash;
    const char                                *ps;
    int                                       pst;

    slash = false;
    cpy_uri = uri;
    if (uri != "/" && uri[uri.length() - 1] == '/' )
        uri.erase(uri.length() - 1);
    while(!uri.empty())
    {
        for (ptr = server.locations.begin(); ptr < server.locations.end(); ptr++)
        {
            if ((*ptr).client_location == "/" && !slash)
            {
                slash = true;
                slash_location = ptr;
            }
            tmp = (*ptr).client_location;
            tmp1 = (*ptr).location_root;
            if (tmp[tmp.length() - 1] == '/')
                tmp.erase(tmp.length() - 1);

            if (tmp == uri)
            {
                if (tmp1[tmp1.length() - 1] == '/' )
                    tmp1.erase(tmp1.length() - 1);
                cpy_uri.erase(0, tmp.length());
                this->path_uri = tmp1 + cpy_uri;
                this->location = (*ptr);
                this->path_cgi = this->location.cgi_path;
                this->accept_cgi = this->location.cgi_accept;
                if (!this->accept_cgi.empty())
                    this->cgi = true;
                return (true);
            }
        }
        ps = std::strrchr(uri.c_str(), '/');
        pst = ps - uri.c_str();
        uri = uri.erase(pst, uri.length() - pst);
    }
    if (slash)
    {
        tmp1 = (*slash_location).location_root;
        if (tmp1[tmp1.length() - 1] == '/' )
                tmp1.erase(tmp1.length() - 1);
        this->path_uri = tmp1 + cpy_uri ;
        this->location = (*slash_location);
        this->path_cgi = this->location.cgi_path;
        if (!this->location.cgi_path.empty())
            this->cgi = true;
        this->accept_cgi = this->location.cgi_accept;
        return (true);
    }
    this->status = "404"; 
    this->override_error_page();
    return (false);
}

void Response::return_301(std::string str)
{
    this->status = "301";
    this->gen_Response.status = NO_BODY__;
    this->gen_Response.headers = "HTTP/1.1 ";
    this->gen_Response.headers +=  "301 " ;
    this->gen_Response.headers += this->status_str["301"] + "\r\n";
    this->gen_Response.headers += "Location: " + str +"\r\n\r\n";
}

bool    Response::delete_all_Contents()
{
    struct stat sb;
    std::string rm;

    if(stat(this->path_uri.c_str(), &sb ) || access(this->path_uri.c_str(), W_OK) != 0)
    {
        this->status = "403"; 
        this->override_error_page();
        return (false);
    }
    if (!((sb.st_mode & S_IRUSR) && (sb.st_mode & S_IWUSR)))
    {
        this->status = "403"; 
        this->override_error_page();
        return (false);
    }
    rm = "rm -rf " + this->path_uri + " 2> /dev/null";
    if (system(rm.c_str()) != 0)
    {
        this->status = "500"; 
        this->override_error_page();
        return (false);
    }
    return (true);
}

bool Response::Is_Dir(struct stat sb)
{
    if (S_ISDIR(sb.st_mode)) //is Directory
    {
        if (this->path_uri[this->path_uri.length() - 1] != '/')
        {
            if (this->headers["method"] == "DELETE")
            {
                this->status = "409";
                this->override_error_page();
            }
            else
                this->return_301(this->headers["uri"] + "/");
        }
        else 
        {
            if (this->headers["method"] == "DELETE")
            {
                if (this->delete_all_Contents())
                {
                    this->status = "204";
                    this->override_error_page();
                }

                return (true);
            }
            if (!this->location.location_index.empty())
            {
                this->fetch_index_files(sb);
            }
        }
        return (true); 
    }
    return (false);
}

void Response::get_requested_resource()
{
    struct stat sb;
    if (stat(this->path_uri.c_str(), &sb ) || (access(this->path_uri.c_str(), R_OK) && this->headers["method"] != "DELETE"))
    {
        this->status = "404";
        this->override_error_page();
    }
    else
    {
        if (!this->Is_Dir(sb))
        {
            this->content_length = sb.st_size;
            this->set_response_file(this->path_uri,sb); 
        }
    }

}

void    Response::upload()
{
    std::string mv;
    mv ="mv " +  this->headers["file_upload"] + " " + this->location.upload_path + " 2> /dev/null";
    if (system(mv.c_str()) != 0)
    {
        this->status = "500"; 
        this->override_error_page();
        return ;
    }
    this->status = "201";
    this->gen_Response.status = ERR__;
    this->headers["Content-type"] = "text/html";
    this->gen_Response.str_err = error_page();
    this->content_length = this->gen_Response.str_err.length();
    (this->gen_Response).headers = this->gen_header("201");
}

void   Response::generate_reponses(tRequest &req)
{
    this->server = getServerPort( gServers, req.serverPort);
    this->gen_Response.id = req.id;
    if (this->check_request(req))
    {
        if (!req.request.path.empty())
        {
            char *ptr= std::strtok((char *)req.request.path.c_str(), "?");
            if (ptr)
                this->headers["uri"] = ptr;
        }
        this->headers["method"] = req.request.method;
        this->headers["connection"] = req.request.connection;
        this->headers["Transfer-Encoding"] = req.request.transfer_encoding;
        this->headers["file_upload"] = req.request.request_body.filename;
        this->content_length = req.request.content_lenght;
        this->max_body_size = this->server->server_cb_size;
        this->gen_Response.pid = -1;
        this->is_content_length = req.request.is_content_length;
        this->query_str = req.request.query_data;
        this->query_path = req.request.query_path;
        this->query_body = req.request.query_body;
        this->content_type = req.request.request_body.content_type;
        if (this->check_request_well_fromed())
        {
            if (this->get_location(*(this->server) ,this->headers["uri"]))
            {
                if (!this->location.return_url.empty())
                {
                    this->return_301(this->location.return_url);
                }
                else
                {
                    if (!this->Method_Not_allowded())
                    {
                       if(this->headers["method"] == "GET")
                       {
                            this->get_requested_resource();
                       }
                        else if (this->headers["method"] == "POST")
                        {
                            if (this->location.upload_path.empty())
                            {
                                this->get_requested_resource();
                            }else
                            {
                                this->upload();
                            }
                        }else
                        {
                            this->get_requested_resource();
                        }
                    }
                }
            }
        }
    }
    // std::cout <<"RESPONSE" << std::endl;
    // std::cout <<gen_Response.status << std::endl;
    // std::cout <<gen_Response.headers << std::endl;
    // std::cout <<gen_Response.str_err << std::endl;


    gResponse.push_back( (this->gen_Response) );

}
Response::~Response()
{
}

void Response::Responses(void){

    while ( ! gRequest.empty() ) 
    {
        this->generate_reponses( gRequest.back() );
        if ( ! gRequest.empty() )
            gRequest.pop_back() ;
        this->clearAll();
    }
}

