#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;
/*Типы лексем.
LEX_LCRO и LEX_RCRO - левая и правая фигурные скобки
*/

template <class T, int max_size> class Stack
{
    T s[max_size];
    int top;
public:
    Stack() { top=0; }
    void reset() { top = 0; }
    void push(T i);
    T pop();
    bool is_empty() { return top == 0; }
    bool is_full() { return top == max_size; }

};

template <class T, int max_size>
void Stack <T, max_size>::push(T i)
{
    if (!is_full())
    {
        s[top] = i;
        ++top;

    }
    else throw "STACK_IS_FULL";

}

template <class T, int max_size>
T Stack <T, max_size> :: pop()
{
    if( !is_empty() )
    {
        --top;
        return s[top];
    }
    else throw "Stack_is_empty";
}

enum type_of_lex
{
    LEX_NULL,
//Служебные слова
    LEX_PROGRAM, LEX_IF, LEX_ELSE,
    LEX_FOR, LEX_WHILE, LEX_BEGIN, LEX_BREAK, LEX_GOTO, LEX_READ, LEX_WRITE, //10
    LEX_INT, LEX_STRING, LEX_NOT, LEX_OR, LEX_AND, LEX_FIN,
    LEX_LABEL, LEX_TRUE, LEX_FALSE, LEX_BOOL, LEX_WRITE_STR, //21
//Разделители
    LEX_LCRO, LEX_RCRO, LEX_LPAREN, LEX_RPAREN, LEX_COLON, LEX_SEMICOLON, LEX_COMMA,
    LEX_EQ, LEX_NEQ, LEX_LEQ, LEX_GEQ, LEX_LSS, LEX_GTR,LEX_SEQ, LEX_SNEQ, LEX_SLEQ, LEX_SGEQ, LEX_SLSS, LEX_SGTR, LEX_PLUS, LEX_SPLUS,
    LEX_MINUS, LEX_UMIN, LEX_DIV, LEX_MUL, LEX_DOT, LEX_ASSIGN, LEX_QUOTE,
//Остальное
    LEX_ID, LEX_NUM, LEX_STR, //45
//ПОЛИЗ
    POLIZ_GO, POLIZ_FGO, POLIZ_LABEL, POLIZ_ADDRESS,
//Служебный элемент перечисления для подсчета кол-ва
    LEX_LAST
};

class lex
{
    type_of_lex t_lex;
    int v_lex;
public:
    lex ( type_of_lex t = LEX_NULL, int v = 0)
    {
        t_lex=t; v_lex=v;
    }
    type_of_lex get_type () {return t_lex;}
    int get_value () {return v_lex;}
    friend ostream& operator << ( ostream &s, lex l)
    {
        s << '(' << l.t_lex << '.' << l.v_lex << ");";
    }
};

class ident
{
    char * name;
    bool   declare;
    type_of_lex type;
    bool   assign;
    int    value;

public:
    ident ()
    {
        declare = false;
        assign = false;
    }

    char * get_name ()
    {
        return name;
    }
    void put_name( const char* n)
    {
        name= new char [strlen(n)+1];
        strcpy(name,n);
    }
    bool get_declare ()
    {
        return declare;
    }
    void put_declare ()
    {
        declare=true;
    }
    type_of_lex get_type()
    {
        return type;
    }
    void put_type(type_of_lex t)
    {
        type=t;
    }
    bool get_assign()
    {
        return assign;
    }
    void put_assign ()
    {
        assign=true;
    }
    int get_value()
    {
        return value;
    }
    void put_value(int v)
    {
        value=v;
    }
};

class tabl_ident
{
    ident * p;
    int size;
    int top;
public:
    tabl_ident (int max_size)
    {
        p = new ident[size=max_size];
        top=1;
    }
    ~tabl_ident ()
    {
        delete []p;
    }
    ident& operator[] (int k)
    {
        return p[k];
    }
    int put (const char* buf);
};

class table_string
{
    char c [20][80];
public:
    table_string()
    {
       for (int i=0; i<20; i++)
            for (int j=0; j<80; j++)
            c[i][j]=NULL;
    }

    int get_free()
    {
        for (int i=0; i<20; i++)
            if (c[i][0]==NULL) return i;
        throw "MEMORY IS FULL";
    }
    void put_str(char* s, int i)
    {
        int n=strlen(s);

        strcpy(c[i],s);
    }
    char* get_str(int i)
    {
        return c[i];
    }

};

class label
{
    char * name;
    int place_to;
    int place_from[10];
    bool is_mark;
    bool is_mark_goto;
    bool is_paste;
public:
    public:
    label ()
    {
        is_mark = false;
        is_paste = false;
        is_mark_goto = false;
        for (int i = 0; i < 10; ++i)
            place_from[i] = -1;
    }

    char * get_name ()
    {
        return name;
    }
    void put_name( const char* n)
    {
        name= new char [strlen(n)+1];
        strcpy(name,n);
    }
    bool get_is_mark ()
    {
        return is_mark;
    }
    void put_is_mark ()
    {
        is_mark=true;
    }
    bool get_is_mark_goto ()
    {
        return is_mark_goto;
    }
    void put_is_mark_goto ()
    {
        is_mark_goto=true;
    }
    bool get_is_paste ()
    {
        return is_paste;
    }
    void put_is_paste ()
    {
        is_paste=true;
    }
    int get_place_to()
    {
        return place_to;
    }
    void put_place_to(int t)
    {
        place_to=t;
    }

    int is_free()
    {
        for ( int i = 0; i < 10; ++i)
        {
            if (place_from[i] == -1)
            {
                return i;
            }
        }
    }
    int get_place_from(int i)
    {
        return place_from[i];
    }
    void put_place_from(int t)
    {
        for (int i = 0; i < 10; ++i)
        {
            if (place_from[i] == -1)
            {
                place_from[i] = t;
                break;
            }
        }
    }
};

class tabl_label
{
    label * p;
    int size;
    int top;
public:
    tabl_label (int max_size)
    {
        p = new label[size=max_size];
        top=1;
    }
    ~tabl_label ()
    {
        delete []p;
    }
    label& operator[] (int k)
    {
        return p[k];
    }
    int put (const char* buf);
};

int tabl_label::put (const char *buf)
{
    for (int j=1;j<top; ++j)
        if (!strcmp(buf,p[j].get_name()))
            return j;
    p[top].put_name(buf);
    ++top;
    return top-1;
}

table_string TS;

class scanner
{
    enum state { H, IDENT, NUMB, ALE, COM, DELIM, EQ, NEQ, UMI, STR};
    static char * TW[];
    static type_of_lex words[];
    static char * TD[];

    int flag_goto = 0;

    static type_of_lex dlms[];
    state CS;
    FILE * fp;
    char c;
    char buf[80];
    int buf_top;

    void clear ()
    {
        buf_top=0;
        for (int j=0; j<80; ++j)
            buf[j]='\0';
    }

    void add ()
    {
        buf [buf_top++] = c;
    }

    int look (const char * buf, char **list )
    {
        int i=0;
        while( list[i] )
        {
            if(!strcmp(buf,list[i]))
                return i;
            ++i;
        }
        return 0;
    }

    void gc ()
    {
        c=fgetc (fp);
       // cout << c << endl;
    }
public:
    lex get_lex();

    scanner (const char * program)
    {
        fp=fopen( program, "r");
        CS=H;
        clear();
        gc();
    }
};

char * scanner::TW[] =
{
    "", "and", "boolean", "break", "else",
    "if", "false", "for", "goto", "int", "not", "or",
    "program", "read", "true", "string", "while",
    "write",NULL
};

char * scanner::TD[] =
{
    "", ";", "@", ",", "=", "(", ")", "==",
    "<", ">", "+", "-", "*", "/", "<=", "!=",
    ">=", "#", "{", "}", "\"", ":", NULL
};

tabl_ident TID(100);
tabl_label TL(100);

type_of_lex scanner::words[]=
{
    LEX_NULL, LEX_AND, LEX_BOOL, LEX_BREAK, LEX_ELSE,
    LEX_IF, LEX_FALSE, LEX_FOR, LEX_GOTO, LEX_INT, LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ,
    LEX_TRUE, LEX_STRING, LEX_WHILE, LEX_WRITE,LEX_NULL
};

type_of_lex scanner::dlms[]=
{
    LEX_NULL, LEX_SEMICOLON, LEX_FIN, LEX_COMMA, LEX_ASSIGN, LEX_LPAREN,
    LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_MUL,
    LEX_DIV, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_UMIN, LEX_LCRO, LEX_RCRO, LEX_QUOTE, LEX_COLON, LEX_NULL
};
class Poliz
{
    lex * p;
    int size;
    int free;
public:
    Poliz( int max_size )
    {
        p = new lex[size = max_size];
        free = 0;
    };
    ~Poliz() { delete[] p; };
    void put_lex(lex l) { p[free] = l; ++free; };
    void put_lex(lex l, int place) { p[place] = l; };
    void blank() {++free;};
    int get_free(){ return free;};
    lex& operator[] (int index)
    {
        if (index > size)
            throw "POLIZ: out of array";
        else if (index > free)
            throw "POLIZ:indefinite element of array";
        else return p[index];
    };

    void print()
    {
        for (int i = 0; i < free; ++i )
        {
            cout << p[i];
        }
    };
};

class Parser
{
    lex            curr_lex;
    type_of_lex    c_type;
    int            c_val;
    scanner        scan;
    Stack <int, 100> st_int;
    Stack <type_of_lex, 100> st_lex;
    Stack <int, 10> st_break;
    int flag_not_if=0;
    int flag_cycle=0;

    void P();
    void D1();
    void D();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F1();
    void F();

    void dec(type_of_lex);
    void check_id ();
    void check_op ();
    void check_not ();
    void eq_type ();
    void eq_bool ();
    void check_id_in_read ();

    void gl()
    {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }

public:
    Poliz prog;

    Parser (const char *program): scan(program), prog(1000) {}

    void analyze();
};

//END ALLHEAD


int tabl_ident::put (const char *buf)
{
    for (int j=1;j<top; ++j)
        if (!strcmp(buf,p[j].get_name()))
            return j;
    p[top].put_name(buf);
    ++top;
    return top-1;
}

lex scanner::get_lex()
{
    int d, j, i;
    CS=H;
    do
    {
        switch ( CS )
        {
        case H:
            if (c==' '||c=='\n'|| c=='\r' || c =='\t')
                gc ();
                else if ( isalpha(c) )
                {
                    clear();
                    add();
                    gc();
                    CS=IDENT;
                }
                else if ( isdigit(c) )
                {
                    d=c-'0';
                    gc();
                    CS=NUMB;
                }
                else if ( c=='<'||c=='>')
                {
                    clear();
                    add();
                    gc();
                    CS=ALE;
                }
                else if ( c==':')
                {
                    gc();
                    return lex(dlms[1],1);
                }
                else if (c == '"')
                {
                    clear();
                    gc();
                    CS=STR;
                }
                else if (c=='=')
                {
                    clear();
                    add();
                    gc();
                    CS=EQ;
                }
                else if (c=='@')
                return lex(LEX_FIN);
                else if (c=='!')
                {
                    clear();
                    add();
                    gc();
                    CS=NEQ;
                }
                else if (c=='/')
                {
                    clear();
                    add();
                    gc();
                    CS=COM;
                }
                else CS=DELIM;
                break;
            case IDENT:
                if(isalpha(c)||isdigit(c))
                {
                    add();
                    gc();
                }
                else
                    if (c == ':')
                {
                    j = TL.put(buf);
                    return lex(LEX_LABEL, j);
                }
                else
                    {

                    if(j=look(buf,TW))
                    {
                        if (j == 8)
                            flag_goto = 1;
                        return lex(words[j],j);
                    }
                    else
                    {
                        if (!flag_goto)
                        {
                            j = TID.put(buf);
                            return lex(LEX_ID, j);
                        }
                        else
                        {
                            j = TL.put(buf);
                            flag_goto = 0;
                            return lex(LEX_LABEL, j);
                        }
                    }
                    }
                break;
            case COM:
                if (c=='/')
                {
                    gc();
                    while(c!='\n')
                    {
                        clear();
                        gc();
                    }
                    CS=H;
                }
                else if (c=='*')
                {
                    int flagcom=0;
                    gc();
                    while(flagcom==0)
                    {
                        clear();
                        gc();
                        if (c=='*') {clear(); gc(); if(c=='/') flagcom=1;}
                    } gc (); CS=H;
                } else { j=look(buf,TD);
                        return lex(dlms[j],j);}

                break;
            case NUMB:
                if(isdigit(c))
                {
                    d=d*10+c-'0';
                    gc();
                }
                else return lex(LEX_NUM, d);
                break;
            case ALE:
                if(c=='=')
                {
                    add();
                    gc();
                    j=look(buf,TD);
                    return lex(dlms[j],j);
                }
                else
                {
                    j=look(buf,TD);
                    return lex(dlms[j],j);
                }
                break;
            case EQ:
                if(c=='=')
                {
                    add();
                    gc();
                    j=look(buf,TD);
                    return lex(dlms[j],j);
                }
                else
                {
                    j=look(buf,TD);
                    return lex(dlms[j],j);

                }
                break;
            case NEQ:
                if(c=='=')
                {
                    add();
                    gc();
                    j=look(buf,TD);
                    return lex(dlms[j],j);
                }
                else
                    throw '!';
                break;
            case STR:
                while (c!='"')
                {
                    add();
                    gc();
                }
                c='\0';
                add();

                i=TS.get_free();

                TS.put_str(buf, i);
                gc();
                return lex(LEX_STR,i);
                break;
            case DELIM:
                clear();
                add();
                if(j=look(buf,TD))
                {
                    gc();
                    return lex(dlms[j],j);
                }
                else
                    throw c;
                break;
        }

    }while (true);
}

void Parser::analyze ()
{
    gl();
    P();
    prog.print();
    cout << endl << "Yes!!!" << endl;
}

void Parser::P ()
{
    if (c_type == LEX_PROGRAM)
      gl();
    else throw curr_lex;
    B();
    gl();
    if (c_type != LEX_FIN)
      throw curr_lex;
}

void Parser::B()
{
    if (c_type == LEX_LCRO)
    {
        gl();
        D1();

        S();

        while ( c_type==LEX_SEMICOLON)
        {

            gl();
            if (c_type==LEX_RCRO) break;
            S();
        }
        if (flag_not_if==1)
        {
            while(!st_break.is_empty())
        {
            prog.put_lex(lex(POLIZ_LABEL,prog.get_free()+2),st_break.pop());
        }
        }
        if (c_type==LEX_RCRO)
            c_type = LEX_SEMICOLON;
        else throw curr_lex;
    } else throw curr_lex;
}

void Parser::D1()
{
while(c_type == LEX_INT || c_type == LEX_BOOL||c_type == LEX_STRING)
    {
        if(c_type == LEX_STRING)
        {
            type_of_lex buf_type=c_type;
            gl();
            D();
            dec(buf_type);
            if(c_type == LEX_SEMICOLON) gl(); else throw curr_lex;
        }
        else {
            type_of_lex buf_type=c_type;
            gl();
            D();
            dec(buf_type);
            if(c_type == LEX_SEMICOLON) gl(); else throw curr_lex;
        }
    }
}

void Parser::D()
{
    st_int.reset();
    if(c_type!=LEX_ID)
        throw curr_lex;
    else
    {
        st_int.push(c_val);
        gl();
        while(c_type == LEX_COMMA)
        {
            gl();
            if (c_type!= LEX_ID)
                throw curr_lex;
            else
            {
                st_int.push(c_val);
                gl();
            }
        }
    }
}

void Parser::S()
{
    int pl0, pl1, pl2, pl3, pl4 , pl5, pl6, pl7;

    if (c_type == LEX_IF)
    {
        gl(); if (c_type!=LEX_LPAREN) throw curr_lex;
        gl();
        E();
        eq_bool();
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex (lex(POLIZ_FGO));
        if (c_type == LEX_RPAREN)
        {
            int cur_flag;
            gl();
            if ( c_type!=LEX_LCRO ) S();
            else {cur_flag=flag_not_if; flag_not_if=0; B(); flag_not_if=cur_flag;}
            pl3 = prog.get_free();
            prog.blank();
            prog.put_lex (lex(POLIZ_GO));
            prog.put_lex (lex(POLIZ_LABEL, prog.get_free()),pl2);
            if(c_type != LEX_SEMICOLON) throw curr_lex;
            gl();
            if (c_type==LEX_ELSE)
            {
                gl();
                if ( c_type!=LEX_LCRO ) S();
                else B();
                prog.put_lex(lex(POLIZ_LABEL, prog.get_free()),pl3);
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    } //end_if
    else
    if ( c_type == LEX_WHILE)
    {
        int cur_cycle = flag_cycle;
        flag_cycle = 1;
        int cur_flag;
        pl0=prog.get_free();
        gl(); if ( c_type!=LEX_LPAREN) throw curr_lex;
        gl();
        E();
        eq_bool();
        pl1=prog.get_free();
        prog.blank();
        prog.put_lex (lex(POLIZ_FGO));
        if ( c_type == LEX_RPAREN )
        {
            gl();
            cur_cycle = flag_cycle;
            if ( c_type!=LEX_LCRO ) S();
            else { cur_flag=flag_not_if; flag_not_if=1; B(); }
            flag_not_if=cur_flag;
            prog.put_lex (lex(POLIZ_LABEL, pl0));
            prog.put_lex (lex(POLIZ_GO));
            prog.put_lex (lex(POLIZ_LABEL,prog.get_free()),pl1);
        }
        else throw curr_lex;
        flag_cycle = cur_cycle;
    } //end while
    else
    if ( c_type == LEX_FOR )
    {
        flag_cycle = 1;
        int cur_flag;
        gl(); if ( c_type!=LEX_LPAREN) throw curr_lex;
        gl();
        S();
        if ( c_type!=LEX_SEMICOLON) throw curr_lex;
        pl4 = prog.get_free();
        gl();
        E();
        eq_bool();
        pl5=prog.get_free();
        prog.blank();
        prog.put_lex (lex(POLIZ_FGO));
        if ( c_type!=LEX_SEMICOLON) throw curr_lex;
        pl6 = prog.get_free();
        prog.blank();
        prog.put_lex (lex(POLIZ_GO));
        gl();
        pl7 = prog.get_free();
        S();
        prog.put_lex(lex(POLIZ_LABEL, pl4));
        prog.put_lex (lex(POLIZ_GO));

        prog.put_lex (lex(POLIZ_LABEL,prog.get_free()),pl6);

        if ( c_type == LEX_RPAREN )
        {
            gl();
            if ( c_type!=LEX_LCRO ) S();
            else { cur_flag=flag_not_if; flag_not_if=1; B(); }
            flag_not_if=cur_flag;
            prog.put_lex (lex(POLIZ_LABEL, pl7));
            prog.put_lex (lex(POLIZ_GO));

            prog.put_lex (lex(POLIZ_LABEL,prog.get_free()),pl5);
        }
        else    throw curr_lex;
        flag_cycle = 0;
    }//end for
    else
    if ( c_type == LEX_READ )
    {
        gl();
        if (c_type == LEX_LPAREN)
        {
            gl();
            if ( c_type==LEX_ID )
            {
                check_id_in_read();
                prog.put_lex (lex (POLIZ_ADDRESS, c_val) );
                gl();
            }
            else throw curr_lex;
            if ( c_type == LEX_RPAREN )
            {
                gl();
                prog.put_lex (lex(LEX_READ));
            }
            else throw curr_lex;
        }
        else throw curr_lex;
    } //end_read
    else
    if ( c_type == LEX_WRITE )
    {
        gl();
        if (c_type==LEX_LPAREN)
        {
            int flagstr=0;
            gl();
            if ((c_type==LEX_STR)||(c_type==LEX_ID && TID[c_val].get_type()==LEX_STRING)) flagstr=1;
            E();
            if (c_type==LEX_COMMA || c_type==LEX_RPAREN)
                if (flagstr==0) prog.put_lex(lex(LEX_WRITE));
                else prog.put_lex(lex(LEX_WRITE_STR));
            else throw curr_lex;

            while (c_type==LEX_COMMA)
            {
                gl();
                E();
                if (c_type==LEX_COMMA || c_type==LEX_RPAREN)
                    if (flagstr==0) prog.put_lex(lex(LEX_WRITE));
                    else prog.put_lex(lex(LEX_WRITE_STR));
                else throw curr_lex;
            }
        } else throw curr_lex;
        if (c_type==LEX_RPAREN)
            gl();
        else throw curr_lex;
    } //end write
    else
    if (c_type == LEX_BREAK)
    {
        if (flag_cycle == 0) throw "breaking bad";
        st_break.push(prog.get_free());
        prog.blank();
        prog.put_lex(lex(POLIZ_GO));
        gl();
    }
    else
    if ( c_type == LEX_ID)
    {
        check_id();
        prog.put_lex (lex(POLIZ_ADDRESS, c_val));
        gl();
        if( c_type == LEX_ASSIGN )
        {
            gl();
            E(); eq_type();
            prog.put_lex (lex (LEX_ASSIGN));
        }
        else throw curr_lex;
    } //assign-end
    else
    if (c_type == LEX_LABEL)
    {
        if (TL[c_val].get_is_mark_goto())
        {
            //prog.put_lex( lex(POLIZ_LABEL,prog.get_free() ), TL[c_val].get_place_from() );
            for ( int i = 0; i < TL[c_val].is_free(); ++i)
            {
                prog.put_lex( lex(POLIZ_LABEL,prog.get_free() ), TL[c_val].get_place_from(i) );
            }
            TL[c_val].put_place_to(prog.get_free());
            TL[c_val].put_is_mark();
        }
        else
        {
            if (!TL[c_val].get_is_mark())
            {
                TL[c_val].put_place_to(prog.get_free());
                TL[c_val].put_is_mark();
            }
            else
                throw "multi label";
        }

        gl();
    }
    else
    if (c_type == LEX_GOTO)
    {
        gl();
        if ( c_type != LEX_LABEL)
            throw "label exepected";
        if ( TL[c_val].get_is_mark() == true )
        {
            prog.put_lex( lex(POLIZ_ADDRESS, TL[c_val].get_place_to() ) );
            prog.put_lex( lex(POLIZ_GO));

        }
        else
        {
            TL[c_val].put_place_from( prog.get_free());
            prog.put_lex(lex(POLIZ_LABEL, -1));
            prog.put_lex(lex(POLIZ_GO));
            TL[c_val].put_is_mark_goto();
        }
        gl();
    }
    else B();
}

void Parser::E()
{
    E1();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
        c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ)
    {
        type_of_lex n=c_type;
        st_lex.push (c_type);
        gl();
        if ((c_type==LEX_STR)||(c_type==LEX_ID && TID[c_val].get_type()==LEX_STRING))
        {
            switch(n)
            {
                case LEX_EQ: n=LEX_SEQ; break;
                case LEX_LSS: n=LEX_SLSS; break;
                case LEX_GTR: n=LEX_SGTR; break;
                case LEX_LEQ: n=LEX_SLEQ; break;
                case LEX_GEQ: n=LEX_SGEQ; break;
                case LEX_NEQ: n=LEX_SNEQ; break;
            }
        }
        E1();
        check_op();
        prog.put_lex(n);
    }
}

void Parser::E1()
{
    T();
    while ( c_type==LEX_PLUS || c_type==LEX_MINUS || c_type==LEX_OR )
    {
        type_of_lex n=c_type;
        st_lex.push (c_type);
        gl();
        if ((c_type==LEX_STR)||(c_type==LEX_ID && TID[c_val].get_type()==LEX_STRING))
        {
            switch(n)
            {
                case LEX_PLUS: n=LEX_SPLUS; break;
                default: throw "wrong types are in operation"; break;
            }
        }
        T();
        check_op();
        prog.put_lex(n);

    }
}

void Parser::T()
{
    F1();
    while ( c_type==LEX_MUL || c_type==LEX_DIV || c_type==LEX_AND )
    {
        type_of_lex n=c_type;
        st_lex.push (c_type);
        gl();
         if ((c_type==LEX_STR)||(c_type==LEX_ID && TID[c_val].get_type()==LEX_STRING))
        {
            throw "wrong types are in operation";

        }
        F1();
        check_op();
        prog.put_lex(n);
    }
}

void Parser::F1()
{
    if(c_type==LEX_MINUS)
    {
    gl();
    F();
    prog.put_lex (lex (LEX_UMIN));
    }
    else
    F();
}

void Parser::F()
{
    if( c_type==LEX_STR)
    {
        st_lex.push( LEX_STRING );
        prog.put_lex( curr_lex );
        gl();

    }
    else
    if( c_type==LEX_ID )
    {
        check_id();
        prog.put_lex (lex (LEX_ID, c_val));
        gl();
    }
    else
    if ( c_type==LEX_NUM )
    {
        st_lex.push( LEX_INT );
        prog.put_lex( curr_lex );
        gl();
    }
    else
    if ( c_type==LEX_TRUE )
    {
        st_lex.push( LEX_BOOL );
        prog.put_lex ( lex(LEX_TRUE, 1));
        gl();
    }
    else
    if ( c_type==LEX_FALSE )
    {
        st_lex.push( LEX_BOOL );
        prog.put_lex ( lex(LEX_FALSE, 0));
        gl();
    }
    else
    if ( c_type==LEX_NOT )
    {
        gl();

        F();
        check_not();
    }
    else
    if ( c_type==LEX_LPAREN )
    {
        gl();
        E();
        if ( c_type==LEX_RPAREN )
            gl();
        else
            throw curr_lex;
    }
    else throw curr_lex;
}

void Parser::dec(type_of_lex type)
{
    int i;
    while ( !st_int.is_empty() )
    {
        i = st_int.pop();
        if (TID[i].get_declare() )
            throw "twice";
        else
        {
            TID[i].put_declare();
            TID[i].put_type(type);
        }
        if (type == LEX_STRING)
        {
            int j = TS.get_free();
            TID[i].put_value(j);
        }
    }
}

void Parser::check_id()
{
    if (TID[c_val].get_declare() )
        st_lex.push(TID[c_val].get_type());
    else throw "not declared";
}

void Parser::check_op()
{
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL, q = LEX_STRING;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();

    if(op==LEX_PLUS || op==LEX_MINUS || op==LEX_MUL || op==LEX_DIV)
    {
        r = t2;
        q = t2;
    }
    if(op==LEX_OR || op==LEX_AND)
    {
        t = LEX_BOOL;
        q = LEX_BOOL;
    }

    if (t1 == t2 && (t1 == t)||(t1 == q))
        st_lex.push(r);
    else throw "wrong types are in operation";
}

void Parser::check_not()
{
    if(st_lex.pop() != LEX_BOOL)
        throw "wrong type is in not";
    else
    {
        st_lex.push(LEX_BOOL);
    }
}

void Parser::eq_type()
{
    if(st_lex.pop() != st_lex.pop() ) throw "wrong types are in =";
}

void Parser::eq_bool()
{
    if (st_lex.pop()!=LEX_BOOL )
        throw "expression is not boolean";
}

void Parser::check_id_in_read()
{
    if (!TID[c_val].get_declare() )
        throw "not declared";
}

class Executer
{
    lex pc_el;
public:
    void execute( Poliz& prog);
};

void Executer::execute (Poliz& prog)
{
    int s2, s1, len, len2;
    Stack <int, 100> args;
    int i, j, index = 0, size = prog.get_free();
    char buf[80];
    while (index < size)
    {
        pc_el = prog[index];

        switch (pc_el.get_type() )
        {
        case LEX_TRUE:
        case LEX_FALSE:
        case LEX_NUM:
        case LEX_STR:
        case POLIZ_ADDRESS:
        case POLIZ_LABEL:
            args.push( pc_el.get_value() );
            break;
        case LEX_ID:
            i = pc_el.get_value();
            if (TID[i].get_assign() )
            {
                args.push(TID[i].get_value() );
                break;
            }
            else throw "POLIZ:indefinite identifier";
        case LEX_NOT:
            args.push(!args.pop() );
            break;
        case LEX_OR:
            i = args.pop();
            args.push(args.pop()||i);
            break;
        case LEX_AND:
            i = args.pop();
            args.push(args.pop() && i);
            break;
        case POLIZ_GO:
            i = args.pop();
            if (i == -1) throw "undefined label";
            index = i - 1;
            break;
        case POLIZ_FGO:
            i = args.pop();

            if(!args.pop() )
                index = i-1;
            break;
        case LEX_WRITE:
            i = args.pop();
            cout << i << endl;
            break;
        case LEX_WRITE_STR:
            i = args.pop();
            cout << TS.get_str(i) << endl;
            break;
        case LEX_READ:
            {
                int k,l=0; char c, buf[20];
                i = args.pop();
                if (TID[i].get_type() == LEX_INT)
                {
                    cout << "Input int value for ";
                    cout << TID[i].get_name()<<endl;
                    cin >> k;

                }
                else if (TID[i].get_type() == LEX_BOOL)
                {
                    char j[20];
                rep:
                    cout << "Input boolean value";
                    cout << "(true or false) for ";

                    cout << TID[i].get_name()<<endl;

                    cin >> j;
                    if (!strcmp(j, "true") ) k=1;
                    else if (!strcmp(j, "false") ) k=0;
                    else {
                        cout <<"ERROR" << endl;
                        goto rep;
                    }
                }
                else
                {
                    char j[20];
                    cout << "Input string value for ";
                    cout << TID[i].get_name()<<endl;
                    cin>>c;
                    while (c!='.')
                    {

                        buf[l]=c;
                        l++;
                        cin >> c;
                    }
                    buf[l]='\0';
                    k=TS.get_free();
                    TS.put_str(buf,k);
                }
                TID[i].put_value(k);
                TID[i].put_assign();
                break;

            }
        case LEX_PLUS:
            args.push(args.pop() + args.pop());
            break;
        case LEX_SPLUS:
            j=TS.get_free(); s2=args.pop(); s1=args.pop(); len; len2;
            len=strlen(TS.get_str(s1)); len2=strlen(TS.get_str(s2));
            strcpy(buf, TS.get_str(s1));
            strcpy(buf+len,TS.get_str(s2));
            buf[len+len2]='\0';
            TS.put_str(buf,j);
            args.push(j);
            break;

        case LEX_UMIN:
            args.push(-1*args.pop());
            break;
        case LEX_MUL:
            args.push(args.pop()*args.pop());
            break;
        case LEX_MINUS:
            i=args.pop();
            args.push( args.pop() - i );
            break;
        case LEX_DIV:
            i=args.pop();
            if (i)
            {
                args.push(args.pop()/i);
                break;
            }
            else throw "POLIZ: divide by zero";
        case LEX_EQ:
            args.push(args.pop() == args.pop());

            break;
        case LEX_SEQ:
            args.push( strcmp(TS.get_str(args.pop()),TS.get_str(args.pop())) == 0 );

            break;
        case LEX_LSS:
            i=args.pop();
            args.push(args.pop()<i);
            break;
        case LEX_SLSS:
            i=args.pop();
             args.push( strcmp(TS.get_str(args.pop()),TS.get_str(i)) < 0 );
            break;
        case LEX_GTR:
            i=args.pop();
            args.push(args.pop()>i);
            break;
        case LEX_SGTR:
            i=args.pop();
             args.push( strcmp(TS.get_str(args.pop()),TS.get_str(i)) > 0 );
            break;
        case LEX_LEQ:
            i=args.pop();
            args.push(args.pop()<=i);
            break;
        case LEX_SLEQ:
            i=args.pop();
             args.push( strcmp(TS.get_str(args.pop()),TS.get_str(i)) <= 0 );
            break;
        case LEX_SGEQ:
            i=args.pop();
             args.push( strcmp(TS.get_str(args.pop()),TS.get_str(i)) >= 0 );
            break;
        case LEX_NEQ:
            i=args.pop();
             args.push( strcmp(TS.get_str(args.pop()),TS.get_str(i)) != 0 );
            break;
        case LEX_ASSIGN:
            i = args.pop();
            j = args.pop();
            TID[j].put_value(i);
            TID[j].put_assign();
            break;
        default:
            throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    }//end of while
    cout << "FINISH of executing !!!"<<endl;
};

class Interpretator
{
    Parser pars;
    Executer E;
public:
    Interpretator (char*program):pars(program) {};
    void interpretation ();
};

void Interpretator::interpretation ()
{
    pars.analyze();
    E.execute (pars.prog);
}

int main ()
{
    try
    {
        Interpretator I("program.txt");
        I.interpretation();
        return 0;
    }

    catch (char c)
    {
        cout<<"unexpected symbol" << c << endl;
        return 1;
    }
    catch (lex l)
    {
        cout << "unexpected lexeme";
        cout << l;
        cout <<endl;
        return 1;
    }
    catch ( const char*source)
    {
        cout << source << endl;
        return 1;
    }
}
