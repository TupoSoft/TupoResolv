#include "dns_answer.hpp"

#include "gtest/gtest.h"

#include <span>

using namespace tuposoft;

TEST(dns_answer, parse_rdata_a) {
    constexpr auto expected = "178.151.191.58";
    auto span = std::span{"\262\227\277:"};
    auto input = std::istringstream{{span.begin(), span.end()}, std::ios::binary};
    const auto actual = parse_rdata<dns_record_e::A>(input);
    ASSERT_EQ(expected, actual);
}

TEST(dns_answer, parse_rdata_aaaa) {
    constexpr auto expected = "2606:2800:21f:cb07:6820:80da:af6b:8b2c";
    auto span = std::span{"&\006(\000\002\037\313\ah \200\332\257k\213,"};
    auto input = std::istringstream{{span.begin(), span.end()}, std::ios::binary};
    const auto actual = parse_rdata<dns_record_e::AAAA>(input);
    ASSERT_EQ(expected, actual);
}

TEST(dns_answer, parse_rdata_ns) {
    constexpr auto expected = "gabriella.ns.cloudflare.com";
    constexpr auto response =
            std::span{"\221X\201\200\000\001\000\002\000\000\000\001\btuposoft\003com\000\000\002\000\001\300\f"
                      "\000\002\000\001\000\001Q\200\000\031\bbenedict\002ns\ncloudflare\300\025\300\f\000\002\000"
                      "\001\000\001Q\200\000\f\tgabriella\3003\000\000)\004\320\000\000\000\000\000\000"};

    const auto response_str = std::string{response.begin(), response.end()};
    const auto offset = response_str.find("\tgabriella");
    ASSERT_NE(offset, std::string::npos);

    auto input = std::istringstream{response_str, std::ios::binary};
    input.seekg(static_cast<std::streamoff>(offset));
    const auto actual = parse_rdata<dns_record_e::NS>(input);
    ASSERT_EQ(expected, actual);
}
