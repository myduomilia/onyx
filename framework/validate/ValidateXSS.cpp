#include "ValidateXSS.h"

std::string onyx::validate::ValidateXSS::stripXSS(const std::string & source) {
    std::string dest;
    boost::regex re("<script>(.*?)</script>");
    dest = boost::regex_replace(source, re, "|");
    re = boost::regex("src[\r\n]*=[\r\n]*\\\'(.*?)\\\'");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("src[\r\n]*=[\r\n]*\\\"(.*?)\\\"");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("</script>");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("<script(.*?)>");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("eval\\((.*?)\\)");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("expression\\((.*?)\\)");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("javascript:");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("vbscript:");
    dest = boost::regex_replace(dest, re, "|");
    re = boost::regex("onload(.*?)=");
    dest = boost::regex_replace(dest, re, "|");
    return dest;
}

std::string onyx::validate::ValidateXSS::escapeXSS(const std::string& source) {
    std::stringstream dest;
    for(size_t i = 0; i < source.length(); i++){
        if(source[i] == '&')
            dest << "&amp;";
        else if(source[i] == '<')
            dest << "&lt;";
        else if(source[i] == '>')
            dest << "&gt;";
        else if(source[i] == '"')
            dest << "&quot;";
        else if(source[i] == '\'')
            dest << "&#39;";
        else 
            dest << source[i];
    }
    return dest.str();
}