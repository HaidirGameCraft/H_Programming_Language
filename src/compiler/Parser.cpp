
#include "Parser.hpp"
#include "Token.hpp"
#include "Function.hpp"
#include "Array.hpp"
#include <cassert>

// #include <execution>
#include <string>
#include <tools/memory.h>
#include <asrt.h>

vector<Token*> parserExpression( vector<Token*> tokens );
vector<Token*> checkBrackets( vector<Token*> tokens );
void checkCondition( Token* parser );

bool Parser::make_easy_parser = true;
Token* Parser::ParserExp(vector<Token*> tokens) {
    //Tokenizer::AnalysisTokenizer(tokens);
    if( tokens.size() == 0 )
        return nullptr;

    int attempts = 30;
    int length = tokens.size();
    while( length > 1 )
    {
        tokens = parserExpression(tokens);
        length = tokens.size();

        EXITFAIL( attempts != 0, {
            printf("[Parser Error]: Attempts of Parser are going to Limited, it might have something wrong\n");
            printf("Size of Tokens: %i\n", tokens.size() );
            for( Token* token : tokens )
            {
                if( token ==  nullptr ) {
                    printf("[Token Null]: Token is NULL");
                    break;
                }
                token->print(0);
            }
        })
        attempts = attempts - 1;
    }

    if( length != 1 )
    {
        printf("Token -> Parser Error: the lenght is not 1 ");
        assert(length == 1);
    }

    //checkCondition( tokens.front() );
    return tokens.front();
}

vector<Token*> parserExpression( vector<Token*> tokens ) {
    Tokenizer::AnalysisBrackets( tokens );
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
        if( (token->getType() == TokenType_OperatorType || token->getType() == TokenType_AssignType ) )
        {
            if( token->left != nullptr || token->right != nullptr )
                continue;
            
            int power_operator = Parser::PrecedenceOperators( token->getName() );
            if( higher_power < power_operator )
            {
                higher_power = power_operator;
                operatorToken = token;
                idx_token = i;
                idx_left_token = ( i - 1 < 0 ) ? 0 : i - 1;
                idx_right_token = ( i + 1 >= tokens.size() ) ? tokens.size() - 1 : i + 1;

                EXITFAIL(i - 1 >= 0, printf("TokenError: Could not find left token") )
                EXITFAIL(i + 1 < tokens.size(), printf("TokenError: Could not find right token") )

            }
        } else if ( 
                    token->getType() == TokenType_FunctionType && 
                    i + 1 < tokens.size() && tokens[i + 1]->getName() == "(" 
                )
        {
            FunctionToken* func = (FunctionToken*) token;

            EXITFAIL( i + 1 < tokens.size() && tokens[i + 1]->getType() == TokenType_BracketType && tokens[i + 1]->getName() == "(", printf("[Syntax Error]: it is not an Function because it doesnt have any '('\n") )
            BracketToken* bracket_tk = ( BracketToken* ) tokens[i + 1];

            openbracket_idx = i + 1;
            closebracket_idx = bracket_tk->closeBracket;

            
            for( int _j = openbracket_idx + 1, __start = _j; _j <= closebracket_idx; _j++ ) {
                if( tokens[_j]->getName() == "," || tokens[_j]->getName() == ")" )
                {
                    vector<Token*> __tmp;
                    for( int  _m = __start; _m < _j; _m++ )
                        __tmp.push_back( tokens[_m] );
                    
                    Token* parser = Parser::ParserExp( __tmp );
                    func->args.push_back( parser );
                    __tmp.clear();
                    
                    __start = _j + 1;
                } else if ( tokens[_j]->getName() == "(" )
                    _j = ((BracketToken*) tokens[_j])->closeBracket;
            }
            break;
        }
        // variable with index
        else if ( token->getType() == TokenType_VariableType && i + 1 < tokens.size() && tokens[i + 1]->getName() == "[")
        {
            BracketToken* bracket_tk = (BracketToken*) tokens[i + 1];
            VariableToken* var_token = ( VariableToken* ) token;    
            openbracket_idx = i + 1; // Open bracket position
            closebracket_idx = bracket_tk->closeBracket; // Close bracket position

            std::vector<Token*> array_index_token;
            int index = openbracket_idx + 1;
            while( index < closebracket_idx ) {
                array_index_token.push_back( tokens[index] );
                index++;
            }

            // parser Index token
            if( array_index_token.size() > 0 )
            {
                Token* indexToken = Parser::ParserExp( array_index_token );
                var_token->indexToken = indexToken;
            }
            
            break;
        }
        else if ( token->getType() == TokenType_BracketType && token->getName() == "[" ) {
            // Array Item
            // Eg: [0, 1, 2, 3, 4], [0]
            BracketToken* bracket_token = (BracketToken*) token;
            openbracket_idx = i + 1;
            closebracket_idx = bracket_token->closeBracket;

            __free( token );
            ArrayListToken* token = (ArrayListToken*) __malloc( sizeof( ArrayListToken ) );
            token->setType( TokenType_ArrayType );
            tokens[i] = token;

            int index = i + 1;
            vector<Token*> __tmp;
            while( index <= closebracket_idx )
            {
                if( tokens[index]->getName() == "," || tokens[index]->getName() == "]" )
                {
                    Token* parser = Parser::ParserExp( __tmp );
                    token->tokens.push_back( parser );
                    __tmp.clear();
                } else {
                    __tmp.push_back( tokens[index] );
                }
                index++;
            }

            break;
        }
        else if ( token->getType() == TokenType_BracketType && token->getName() == "(" )
        {
            // if( token->left != nullptr || token->right != nullptr )
            //     continue;

            BracketToken* bracket_tk = (BracketToken*) token;
            std::vector<Token*> __tmp;
            openbracket_idx = i + 1;
            closebracket_idx = bracket_tk->closeBracket;

            int index = i + 1;
            while( index < closebracket_idx )
            {
                __tmp.push_back( tokens[index] );
                index++;
            }


            __free( token );
            tokens[openbracket_idx - 1] = Parser::ParserExp( __tmp );
            __tmp.clear();

            break;
        }
    }

    vector<Token*> new_tokens;
    for(int i = 0; i < tokens.size(); i++) {
        if( i == openbracket_idx )
        {
            // Skip/Ignore
            i = closebracket_idx;
        } else if ( i >= idx_left_token && i <= idx_right_token )
        {
            if( i == idx_token )
            {
                assert( operatorToken != nullptr );
                operatorToken->left = tokens[idx_left_token];
                operatorToken->right = tokens[idx_right_token];
 
                if( Parser::make_easy_parser && operatorToken->left->getType() == TokenType_NumberType && operatorToken->right->getType() == TokenType_NumberType )
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

                if( operatorToken->getName() == "&&" )
                {
                    if( operatorToken->left->getType() == TokenType_OperatorType )
                    {
                        if( operatorToken->left->getName() == "==" ) operatorToken->left->setName("!=");
                        else if( operatorToken->left->getName() == "<" ) operatorToken->left->setName(">");
                        else if( operatorToken->left->getName() == ">" ) operatorToken->left->setName("<");
                        else if( operatorToken->left->getName() == "<=" ) operatorToken->left->setName(">=");
                        else if( operatorToken->left->getName() == ">=" ) operatorToken->left->setName("<=");
                        else if( operatorToken->left->getName() == "!=" ) operatorToken->left->setName("==");
                    }
                    
                    if( operatorToken->right->getType() == TokenType_OperatorType )
                    {
                        if( operatorToken->right->getName() == "==" ) operatorToken->right->setName("!=");
                        else if( operatorToken->right->getName() == "<" )  operatorToken->right->setName(">");
                        else if( operatorToken->right->getName() == ">" )  operatorToken->right->setName("<");
                        else if( operatorToken->right->getName() == "<=" ) operatorToken->right->setName(">=");
                        else if( operatorToken->right->getName() == ">=" ) operatorToken->right->setName("<=");
                        else if( operatorToken->right->getName() == "!=" ) operatorToken->right->setName("==");
                    }
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
                    assert( false && "BracketError: could not find the start of '(' or '['");
                }

                BracketToken* start_brk_tk = bracket_idx[ bracket_idx.size() - 1 ];
                bracket_idx.pop_back();

                start_brk_tk->closeBracket = i;
            }
        }
    }

    if( bracket_idx.size() != 0 )
    {
        printf("Size of Bracket idx: %i\n", bracket_idx.size() );
        assert( false && "BracketError: could not reach the end of ')' or ']'" );
    }

    return tokens;
}

void checkCondition( Token* parser ) {
   if( parser->getType() == TokenType_OperatorType )
   {
       if( parser->getName() == "&&" )
       {
            if( parser->left != nullptr && parser->left->getType() == TokenType_OperatorType ) { 
                if ( parser->left->getName() == "==" ) parser->left->setName("!=");
                else if ( parser->left->getName() == "<"  ) parser->left->setName( ">");
                else if ( parser->left->getName() == ">"  ) parser->left->setName( "<");
                else if ( parser->left->getName() == "<=" ) parser->left->setName(">=");
                else if ( parser->left->getName() == ">=" ) parser->left->setName("<=");
                else if ( parser->left->getName() == "!=" ) parser->left->setName("==");
            }

            if( parser->right != nullptr && parser->right->getType() == TokenType_OperatorType ) { 
                if ( parser->right->getName() == "==" ) parser->right->setName("!=");
                else if ( parser->right->getName() == "<"  ) parser->right->setName( ">");
                else if ( parser->right->getName() == ">"  ) parser->right->setName( "<");
                else if ( parser->right->getName() == "<=" ) parser->right->setName(">=");
                else if ( parser->right->getName() == ">=" ) parser->right->setName("<=");
                else if ( parser->right->getName() == "!=" ) parser->right->setName("==");
            }

            return;
       }
       
       if( parser->left != nullptr && parser->left->getType() == TokenType_OperatorType)
           checkCondition( parser->left );
       if( parser->right != nullptr && parser->right->getType() == TokenType_OperatorType)
           checkCondition( parser->right );
   }
}

int Parser::PrecedenceOperators(const std::string &symbols) {
    if( symbols == "=" ) return 1;
    else if ( symbols == "||" ) return 2;
    else if ( symbols == "&&" ) return 3;
    else if ( symbols == "==" || symbols == "!=" ) return 4;
    else if ( symbols == "<" || symbols == "<=" || symbols == ">" || symbols == ">=" ) return 5;
    else if ( symbols == "+" || symbols == "-" ) return 6;
    else if ( symbols == "*" || symbols == "/" || symbols == "%" ) return 7;

    return 0;
}
