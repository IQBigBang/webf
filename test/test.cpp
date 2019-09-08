#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#define WEBF_TESTING // Makes all members public in Lexer and Parser
#include "Lexer.h"
#include "INode.h"
#include "Parser.h"
#include "WebF_Type.h"
#include "Runtime.h"
#include "ExprNode.hpp"
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
    REQUIRE(t.ttype == CLOSINGTAGEND);
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

TEST_CASE ("Lexer tokenizes string", "[lexer]")
{
    Lexer l(" \"a b c-11 {} b\n\"  ");
    auto t = l.next();
    REQUIRE(t.ttype == STRING);
    REQUIRE(t.tval == "a b c-11 {} b\n");
    REQUIRE(l.pos == 17);
}

TEST_CASE ("Lexer tokenizes numbers", "[lexer]")
{
    Lexer l("1.223 8.45.66");
    auto t = l.next();
    REQUIRE(t.ttype == NUMBER);
    REQUIRE(t.tval == "1.223");
    REQUIRE(l.pos == 5);
    t = l.next();
    REQUIRE(t.ttype == NUMBER);
    REQUIRE(t.tval == "8.45");
    REQUIRE(l.pos == 10);
    t = l.next();
    REQUIRE(t.ttype == DOT);
    t = l.next();
    REQUIRE(t.ttype == NUMBER);
    REQUIRE(t.tval == "66");
    REQUIRE(l.pos == 13);
}

TEST_CASE ("Lexer tokenizes names", "[lexer]")
{
    Lexer l("hello ++:?");
    auto t = l.next();
    REQUIRE(t.ttype == NAME);
    REQUIRE(t.tval == "hello");
    REQUIRE(l.pos == 5);
    t = l.next();
    REQUIRE(t.ttype == NAME);
    REQUIRE(t.tval == "++:?");
    REQUIRE(l.pos == 10);
}

TEST_CASE ("Lexer 'peek' function", "[lexer]")
{
    Lexer l1("set a b c");
    REQUIRE(l1.peek(NAME));
    Lexer l2("( x )");
    REQUIRE(l2.peek(LPAREN));
    Lexer l3("$=x");
    REQUIRE(l3.peek(DOLLAR));
}

TEST_CASE ("Lexer 'peek next' function", "[lexer]")
{
    Lexer l1("(a b c)");
    l1.peek(LPAREN);
    REQUIRE(l1.peekNext(NAME));
    Lexer l2("$=x");
    REQUIRE(l2.peekNext(EQUAL));
}

TEST_CASE ("Lexer 'eat' funtion", "[lexer]")
{
    Lexer l("a.blah");
    l.peek(NAME);
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
    l.eat(); // EQUAL
    l.expect(LPAREN);
    REQUIRE(l.peek(NAME));
    REQUIRE(l.eat() == "+");
    REQUIRE(l.expect(NUMBER) == "2");
}

TEST_CASE ("Parser parsing simple expressions", "[parser]")
{
    Lexer l("(print \"Hello world\")");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( f = var( print ), arg0 = str(Hello world),  )");
}

TEST_CASE ("Parser parsing simple lists and blocks", "[parser]")
{
    Lexer l("(blah [1 2 blah] {(a 3) (b 2)})");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( f = var( blah ), arg0 = list( 0 = num(1), 1 = num(2), 2 = var( blah ),  ), arg1 = block( func( f = var( a ), arg0 = num(3),  ), func( f = var( b ), arg0 = num(2),  ),  ),  )");
}

TEST_CASE ("Parser parsing get and vardef expressions", "[parser]")
{
    Lexer l("(args= [$a $b])");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( f = var( set ), arg0 = vardef( args ), arg1 = list( 0 = vardef( a ), 1 = vardef( b ),  ),  )");
}

TEST_CASE ("Parser parsing member access", "[parser]")
{
    Lexer l("(inlen= (io.readline).len)");
    Parser p(l);
    ExprNode* e = p.parse_expr(); 
    REQUIRE(e->repr() == "func( f = var( set ), arg0 = vardef( inlen ), arg1 = member( object = func( f = member( object = var( io ), name = readline ),  ), name = len ),  )");
}


TEST_CASE ("Parser parsing elements", "[parser]")
{
    Lexer l("(set $a <div><example /></div>)");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( f = var( set ), arg0 = vardef( a ), arg1 = element( elname = div, args = (  ), children = (element( elname = example, args = (  ), children = ( )),  )),  )");
}

TEST_CASE ("Parser parsing attributes", "[parser]")
{
    Lexer l("(set $el <span attr1=\"Static value\" attr2=varval attr3=(call.f 1 2) />)");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( f = var( set ), arg0 = vardef( el ), arg1 = element( elname = span, args = ( attr1 = str(Static value); attr2 = var( varval ); attr3 = func( f = member( object = var( call ), name = f ), arg0 = num(1), arg1 = num(2),  );  ), children = ( )),  )");
}


TEST_CASE ("Parser parsing text nodes", "[parser]")
{
    Lexer l("(set el <span>Hello, $name ! You are X yo. <table><td/></table></span>)");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( f = var( set ), arg0 = var( el ), arg1 = element( elname = span, args = (  ), children = (text( Hello, $name ! You are X yo.  ), element( elname = table, args = (  ), children = (element( elname = td, args = (  ), children = ( )),  )),  )),  )");
}

TEST_CASE ("Parser fully working", "[parser]")
{
    Lexer l("(def $render [$x] {(el= <div id=\"el\">%x</div>) (return (el.render))})");
    Parser p(l);
    ExprNode* e = p.parse_expr();
    REQUIRE(e->repr() == "func( f = var( def ), arg0 = vardef( render ), arg1 = list( 0 = vardef( x ),  ), arg2 = block( func( f = var( set ), arg0 = vardef( el ), arg1 = element( elname = div, args = ( id = str(el);  ), children = (text( %x ),  )),  ), func( f = var( return ), arg0 = func( f = member( object = var( el ), name = render ),  ),  ),  ),  )");
}
// Handle exception throwing tests