
#include "Parser.hpp"
#include "Token.hpp"
#include "Function.hpp"
#include <cassert>

#include <string>
#include <tools/memory.h>

int operator_power(Token* token);
vector<Token*> parserExpression( vector<Token*> tokens );
vector<Token*> checkBrackets( vector<Token*> tokens );

Token* Parser::ParserExp(vector<Token*> tokens) {
    if( tokens.size() == 0 )
        return nullptr;

    int length = tokens.size();
    while( length > 1 )
    {
        tokens = parserExpression(tokens);
        length = tokens.size();
    }

    if( length != 1 )
    {
        printf("Token -> Parser Error: the lenght is not 1 ");
        assert(length == 1);
    }

    return tokens.front();
}

vector<Token*> parserExpression( vector<Token*> tokens ) {
    tokens = checkBrackets( tokens );
    //printf("[DEBUG] Checking the Bracket");
    int higher_power = 0;
    int idx_token = -1;
    int idx_left_token = -1;
    int idx_right_token = -1;

    int openbracket_idx = -1; int closebracket_idx = -1;
    Token* operatorToken = nullptr;
    Token* leftToken = nullptr;
    Token* rightToken = nullptr;

    Token* bracketToken = nullptr;
    FunctionToken* functionToken = nullptr;
    ArrayToken* arrayToken = nullptr;

    vector<int> bracket_idx;
    for( int i = 0; i < tokens.size(); i++)
    {
        Token* token = tokens[i];
        if( token->getType() == TokenType_OperatorType || token->getType() == TokenType_AssignType)
        {
            if( token->left != nullptr || token->right != nullptr )
                continue;
            
            int power_operator = operator_power( token );
            if( higher_power < power_operator )
            {
                higher_power = power_operator;
                operatorToken = token;
                idx_token = i;
                idx_left_token = ( i - 1 < 0 ) ? 0 : i - 1;
                idx_right_token = ( i + 1 >= tokens.size() ) ? tokens.size() - 1 : i + 1;

                if( i - 1 < 0 )
                {
                    printf("TokenError: Could not find left token");
                    assert(i - 1 >= 0);
                }

                if( i + 1 >= tokens.size() )
                {
                    printf("TokenError: Could not find right token");
                    assert(i + 1 < tokens.size());
                }

            }
        } else if ( token->getType() == TokenType_FunctionType && ((FunctionToken*) token)->hasSet == false )
        {
            FunctionToken* func = (FunctionToken*) token;
            func->hasSet = true;
            functionToken = func;

            i++;    // skipping the function, get next token
            assert( tokens[i]->getType() == TokenType_BracketType );
            vector<int> brackets_idx;
            BracketToken* bracket_tk = ( BracketToken* ) tokens[i];

            openbracket_idx = i - 1;
            closebracket_idx = bracket_tk->closeBracket;
            brackets_idx.push_back( i ); 
            i++;    // skipping '('

            int start = i;
            while( brackets_idx.size() > 0 && i < tokens.size() )
            {
                Token* __tk = tokens[i];
                if( __tk->getType() == TokenType_BracketType )
                {
                    if( __tk->getName() == "(" )
                        brackets_idx.push_back( i );
                    else if ( __tk->getName() == ")" ) {
                        if( brackets_idx.size() == 1 )
                        {
                            vector<Token*> args__;
                            for(int j = start; j < i; j++)
                                args__.push_back( tokens[j] );


                            if ( args__.size() > 0 ) {
                                Token* parser = Parser::ParserExp( args__ );
                                func->args.push_back( parser );
                            }

                            brackets_idx.pop_back();
                            break;
                        }
                        brackets_idx.pop_back();
                    }
                }
                else if( __tk->getType() == TokenType_SymbolsType &&
                        __tk->getName() == "," )
                {
                    assert( brackets_idx.size() == 1 );
                    vector<Token*> args__;
                    for(int j = start; j < i; j++)
                        args__.push_back( tokens[j] );
                    
                    Token* parser = Parser::ParserExp( args__ );
                    func->args.push_back( parser );
                    start = i + 1;
                }

                i++;
            }

            assert(brackets_idx.size() == 0);
            break;
        }
        else if ( token->getType() == TokenType_ArrayType && !token->hasSet )
        {
            arrayToken = (ArrayToken*) token;
            arrayToken->hasSet = true;
            openbracket_idx = i;    // next Token
            assert( tokens[i + 1]->getType() == TokenType_BracketType && tokens[i + 1]->getName() == "[");
            BracketToken* array_bracket_token = ( BracketToken* ) tokens[i + 1];
            closebracket_idx = array_bracket_token->closeBracket;

            for(int idx = openbracket_idx + 2; idx < closebracket_idx; idx++) {
                arrayToken->tokens.push_back( tokens[idx] );
            }
            break;
        }
        else if ( token->getType() == TokenType_BracketType )
        {
            if( token->left != nullptr || token->right != nullptr )
                continue;

            if( token->getName() != "(" || token->getName() != ")" )
                continue;

            BracketToken* bracket_tk = ( BracketToken* ) token;
            vector<Token*> inside;
            openbracket_idx = i;
            i++;    // skipping bracket token
            while( i < bracket_tk->closeBracket )
            {
                inside.push_back( tokens[i] );
                i++;
            }

            bracketToken = Parser::ParserExp(inside);
            closebracket_idx = bracket_tk->closeBracket;
            break;
        }
    }

    vector<Token*> new_tokens;
    for(int i = 0; i < tokens.size(); i++) {
        if( i >= openbracket_idx && i <= closebracket_idx )
        {
            if(bracketToken != nullptr) {
                new_tokens.push_back( bracketToken );
                bracketToken = nullptr;
            }
            else if ( functionToken != nullptr )
            {
                new_tokens.push_back( functionToken );
                functionToken = nullptr;
            }
            else if ( arrayToken != nullptr ) {
                new_tokens.push_back( arrayToken );
                arrayToken = nullptr;
            }
        } else if ( i >= idx_left_token && i <= idx_right_token && 
                    bracketToken == nullptr && 
                    functionToken == nullptr &&
                    arrayToken == nullptr
                )
        {
            if( i == idx_token )
            {
                assert( operatorToken != nullptr );
                operatorToken->left = tokens[idx_left_token];
                operatorToken->right = tokens[idx_right_token];
 
                if( operatorToken->left->getType() == TokenType_NumberType && operatorToken->right->getType() == TokenType_NumberType )
                {
                    double __val = 0;
                    double __left = stod( operatorToken->left->getName() );
                    double __right = stod( operatorToken->right->getName() );
                    if( operatorToken->getName() == "+"  ) __val = __left + __right;
                    else if ( operatorToken->getName() == "-") __val = __left - __right;
                    else if ( operatorToken->getName() == "*") __val = __left * __right;
                    else if ( operatorToken->getName() == "/") __val = __left / __right;
                    else if ( operatorToken->getName() == "==") __val = __left == __right;
                    else if ( operatorToken->getName() == "!=") __val = __left != __right;
                    else if ( operatorToken->getName() == "<") __val = __left < __right;
                    else if ( operatorToken->getName() == ">") __val = __left > __right;
                    else if ( operatorToken->getName() == "<=") __val = __left <= __right;
                    else if ( operatorToken->getName() == ">=") __val = __left >= __right;
                    else if ( operatorToken->getName() == "&&") __val = ( (int) __left & 1 ) & ((int)__right & 1 );
                    else if ( operatorToken->getName() == "||") __val = ( (int)__left & 1) | ( (int) __right & 1);

                    __free( operatorToken->left );
                    __free( operatorToken->right );
                    operatorToken->left = nullptr;
                    operatorToken->right = nullptr;
                    operatorToken->setName( to_string((int)( __val ) ) );
                    operatorToken->setType(TokenType_NumberType);
                }
                new_tokens.push_back( operatorToken );
            }
        }
        else {
            new_tokens.push_back( tokens[i] );
        }
    }

    return new_tokens;
}

vector<Token*> checkBrackets( vector<Token*> tokens ) {
    vector<BracketToken*> bracket_idx;
    for( int i = 0; i < tokens.size(); i++ )
    {
        Token* token = tokens[i];
        if( token->getType() == TokenType_BracketType )
        {
            BracketToken* brk_tk = (BracketToken*) token;
            if( brk_tk->getName() == "(" || brk_tk->getName() == "[")
                bracket_idx.push_back( brk_tk );
            else if ( brk_tk->getName() == ")" || brk_tk->getName() == "]")
            {
                if( bracket_idx.size() == 0 )
                {
                    printf("BracketError: could not find the start of '(' or '['");
                    assert( bracket_idx.size() != 0 );
                }

                BracketToken* start_brk_tk = bracket_idx[ bracket_idx.size() - 1 ];
                bracket_idx.pop_back();

                start_brk_tk->closeBracket = i;
            }
        }
    }

    if( bracket_idx.size() != 0 )
    {
        printf("BracketError: could not reach the end of ')' or ']'");
        assert( bracket_idx.size() != 0 );
    }

    return tokens;
}

int operator_power(Token* token) {
    if( token->getName() == "+" || token->getName() == "-" )
        return 4;
    else if ( token->getName() == "=" )
        return 3;
    else if ( token->getName() == "*" || token->getName() == "/" )
        return 5;
    else if(    token->getName() == "==" || 
                token->getName() == ">" ||
                token->getName() == "<" ||
                token->getName() == ">=" ||
                token->getName() == "<=" ||
                token->getName() == "!="
            ) {
        return 2;
    }
    else if ( token->getName() == "&&" || token->getName() == "||" )
    {
        return 1;
    }

    return 0;
}


