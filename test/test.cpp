#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#define WEBF_TESTING // Makes all members public in Lexer and Parser
#include "Lexer.h"
#include "AST.h"
#include "Parser.h"
#include "WebF_Type.h"
#include "Runtime.h"
#include "WebF_String.hpp"
#include "WebF_Number.hpp"
#include <iostream>

TEST_CASE ("Lexer reads characters", "[lexer]")
{
    Lexer l("x");
    REQUIRE(l.getCh() == 'x');
    l.pos = 1;
    REQUIRE(l.getCh() == '\x01');
    l.pos = 2;
    REQUIRE(l.getCh() == '\x01');
}

TEST_CASE ("Lexer skips whitespace", "[lexer]")
{
    Lexer l(" \n a  \t  bc");
    l.skipWhitespace();
    REQUIRE(l.pos == 3);
    l.pos = 5;
    l.skipWhitespace();
    REQUIRE(l.pos == 9);
}

TEST_CASE ("Lexer counts lines", "[lexer][!mayfail]")
{
    Lexer l(" \n \r \r\n z");
    l.skipWhitespace();
    REQUIRE(l.lineno == 4);
    REQUIRE(l.linepos == 1);
}

TEST_CASE ("Lexer counts line positions", "[lexer]")
{
    // TODO: Implemet line counting unit test case
}

TEST_CASE ("Lexer tokenizes one-char tokens", "[lexer]")   // Note: this does not test all one char tokens
{
    Lexer l("($.}");
    auto t = l.next();
    REQUIRE(t.ttype == LPAREN);
    REQUIRE(t.tval == "(");
    l.pos = 1;
    t = l.next();
    REQUIRE(t.ttype == DOLLAR);
    REQUIRE(t.tval == "$");
    l.pos = 2;
    t = l.next();
    REQUIRE(t.ttype == DOT);
    REQUIRE(t.tval == ".");
    l.pos = 3;
    t = l.next();
    REQUIRE(t.ttype == RBRACE);
    REQUIRE(t.tval == "}");
}

TEST_CASE ("Lexer tokenizes html tokens", "[lexer]")
{
    Lexer l("<a/><b></b>");
    auto t = l.next();
    REQUIRE(t.ttype == TAGSTART);
    REQUIRE(t.tval == "<");
    l.pos = 2;
    t = l.next();
    REQUIRE(t.ttype == TAGCLOSINGEND);
    REQUIRE(t.tval == "/>");
    l.pos = 6;
    t = l.next();
    REQUIRE(t.ttype == TAGEND);
    REQUIRE(t.tval == ">");
    l.pos = 7;
    t = l.next();
    REQUIRE(t.ttype == CLOSINGTAGSTART);
    REQUIRE(t.tval == "</");
}

TEST_CASE ("Lexer tokenizes quoted string", "[lexer]")
{
    Lexer l(" \"a b c-11 {} b\n\"  ");
    auto t = l.next();
    REQUIRE(t.ttype == STRING);
    REQUIRE(t.tval == "a b c-11 {} b\n");
    REQUIRE(l.pos == 17);
}

TEST_CASE ("Lexer tokenizes string", "[lexer]")
{
    Lexer l("set a_b12:ca4%X");
    auto t = l.next();
    REQUIRE(t.ttype == STRING);
    REQUIRE(t.tval == "set");
    REQUIRE(l.pos == 3);
    t = l.next();
    REQUIRE(t.ttype == STRING);
    REQUIRE(t.tval == "a_b12:ca4%X");
    REQUIRE(l.pos == 15);
}

TEST_CASE ("Lexer 'peek' function", "[lexer]")
{
    Lexer l1("set a b c");
    REQUIRE(l1.peek(STRING));
    Lexer l2("( x )");
    REQUIRE(l2.peek(LPAREN));
    Lexer l3("$=x");
    REQUIRE(l3.peek(DOLLAR));
}

TEST_CASE ("Lexer 'peek next' function", "[lexer]")
{
    Lexer l1("(a b c)");
    l1.peek(LPAREN);
    REQUIRE(l1.peekNext(STRING));
    Lexer l2("$=x");
    REQUIRE(l2.peekNext(EQUAL));
}

TEST_CASE ("Lexer 'eat' funtion", "[lexer]")
{
    Lexer l("a.blah");
    l.peek(STRING);
    REQUIRE(l.eat() == "a");
    REQUIRE(l.peek(DOT));
    REQUIRE(l.eat() == ".");
    REQUIRE(l.eat() == "blah");
}


TEST_CASE ("Lexer 'expect' function", "[lexer]")
{
    Lexer l("{}");
    l.expect(LBRACE);
    l.expect(RBRACE);
}

TEST_CASE ("Lexer fully working", "[lexer]")
{
    Lexer l("(\"result\"= (+ 2 3))");
    REQUIRE(l.peek(LPAREN));
    l.eat();
    REQUIRE(l.expect(STRING) == "result");
    REQUIRE(l.peek(EQUAL));
    REQUIRE(l.peekNext(LPAREN));
    l.eat();
    l.expect(LPAREN);
    REQUIRE(l.peek(STRING));
    REQUIRE(l.eat() == "+");
    REQUIRE(l.expect(STRING) == "2");
}

TEST_CASE ("Parser parsing simple expressions", "[parser]")
{
    Lexer l("(print a)");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( name = str(print), arg0 = str(a),  )");
    delete e;
}

TEST_CASE ("Parser parsing simple lists and blocks", "[parser]")
{
    Lexer l("(def add [a b] { (return (+ a b)) })");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( name = str(def), arg0 = str(add), arg1 = list( 0 = str(a), 1 = str(b),  ), arg2 = block( func( name = str(return), arg0 = func( name = str(+), arg0 = str(a), arg1 = str(b),  ),  ),  ),  )");
    delete e;
}

TEST_CASE ("Parser parsing get and set expressions", "[parser]")
{
    Lexer l("(abc= (* 2 $x))");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( name = str(set), arg0 = str(abc), arg1 = func( name = str(*), arg0 = str(2), arg1 = func( name = str(get), arg0 = str(x),  ),  ),  )");
    delete e;
}

TEST_CASE ("Parser parsing elements", "[parser]")
{
    Lexer l("(set a (<div><example /></div>))");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( name = str(set), arg0 = str(a), arg1 = element( elname = div, args = (  ), children = (element( elname = example, args = (  ), children = ( )),  )),  )");
    delete e;
}

TEST_CASE ("Parser parsing attributes", "[parser]")
{
    Lexer l("(set el (<span attr1=\"val1\" attr2=$x attr3=$(func x) attr4={(func x)}/>))");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( name = str(set), arg0 = str(el), arg1 = element( elname = span, args = ( attr1 = str(val1); attr2 = func( name = str(get), arg0 = str(x),  ); attr3 = func( name = str(func), arg0 = str(x),  ); attr4 = block( func( name = str(func), arg0 = str(x),  ),  );  ), children = ( )),  )");
    delete e;
}

TEST_CASE ("Parser parsing text nodes", "[parser][!mayfail]")
{
    Lexer l("(set el (<span>Hello, $name ! You are 18 years old.</span>))");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "Unknown. TODO");
    delete e;
}

TEST_CASE ("WebF_Number basic arithmetic", "[std_lib][webf_num]")
{
    WebF_Number* n1 = new WebF_Number(22.0);
    WebF_Number* n2 = new WebF_Number(-8.0);
    auto args = new std::vector<IWebF_Type*>();
    args->push_back(n2);
    WebF_Number* res = dynamic_cast<WebF_Number*>(n1->invokeMethod("+", args));
    REQUIRE(res->getnum() == 14.0); // Integer values used because of errors when comparing FP values
    res = dynamic_cast<WebF_Number*>(n1->invokeMethod("-", args));
    REQUIRE(res->getnum() == 30.0);
    res = dynamic_cast<WebF_Number*>(n1->invokeMethod("*", args));
    REQUIRE(res->getnum() == -176.0);
    res = dynamic_cast<WebF_Number*>(n1->invokeMethod("/", args));
    REQUIRE(res->getnum() == -2.75);
    delete n1;
    delete n2;
    delete res;
}

// Handle exception throwing tests
