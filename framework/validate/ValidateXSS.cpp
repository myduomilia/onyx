#include <boost/algorithm/string/replace.hpp>

#include "ValidateXSS.h"

std::string onyx::validate::ValidateXSS::stripXSS(std::string & source) {
    boost::regex re("<script>(.*?)</script>");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("src[\r\n]*=[\r\n]*\\\'(.*?)\\\'");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("src[\r\n]*=[\r\n]*\\\"(.*?)\\\"");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("</script>");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("<script(.*?)>");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("eval\\((.*?)\\)");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("expression\\((.*?)\\)");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("javascript:");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("vbscript:");
    source = boost::regex_replace(source, re, "");
    re = boost::regex("onload(.*?)=");
    source = boost::regex_replace(source, re, "");
    return source;
}