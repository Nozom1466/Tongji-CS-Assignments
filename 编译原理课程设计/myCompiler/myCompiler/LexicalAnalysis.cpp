#pragma execution_character_set("utf-8") 
#include"LexicalAnalysis.h"
#include<sstream>

token::token(string w, int t, int l)
{
    this->word = w;//类型
    this->type = t;//值
    this->line = l;//所在行数
}

int lexer::lexicalAnalysis(string file_name) {

    if ((this->fcode = fopen(file_name.c_str(), "r")) == NULL) {
        return  CODE_FILE_OPEN_ERROR;
    }

    this->ftoken = fopen(TOKEN_LIST_FILE, "w");//存储词法分析结果
    fprintf(this->ftoken, "Token类型    Token值      所在行数\n");
    this->ferror = fopen(ERROR_LIST_FILE, "w");

    char c = '\0';
    int buffer_flag = 0;//缓冲区是否需要轮转
    int line_count = 0;//记录当前行数

    while (1)
    {
        c = fgetc(this->fcode);
        if (c == EOF)//代码读取结束
        {
            line_count++;
            this->deleComments(line_count);// 删除注释
            this->deleSpaces(line_count); // 删除多余空格
            if (this->double_buffer[this->current_buffer].count > 0)
            {
                strcpy(this->final_buffer.buffer, this->double_buffer[this->current_buffer].buffer);
                this->final_buffer.count = this->double_buffer[this->current_buffer].count;
                //进入状态机处理 
                this->double_buffer[current_buffer].count = 0;
                if (!this->DFA(line_count)) {
                    fclose(fcode);
                    fclose(ftoken);
                    fclose(ferror);
                    this->error_line = line_count;
                    return LEXICAL_ANALYSIS_ERROR;
                }
            }
            break;
        }
        //缓冲池满了
        if (this->double_buffer[this->current_buffer].count == BUFFER_SIZE - 2)
        {
            this->double_buffer[this->current_buffer].buffer[this->double_buffer[this->current_buffer].count] = c;
            int i;
            for (i = 0; i < this->double_buffer[this->current_buffer].count; i++)
            {
                if (isDelimiter(double_buffer[this->current_buffer].buffer[i]))
                {
                    int j;//分界点
                    int k;
                    // 转移缓冲区内容
                    for (j = 0, k = i + 1; k <= double_buffer[current_buffer].count; k++, j++)
                    {
                        double_buffer[1 - current_buffer].buffer[j] = double_buffer[current_buffer].buffer[k];
                    }
                    //count大小重新设置
                    double_buffer[1 - current_buffer].count = j;
                    double_buffer[current_buffer].count = i;
                    //设置终结点
                    double_buffer[1 - current_buffer].buffer[j] = '\0';
                    double_buffer[current_buffer].buffer[i + 1] = '\0';
                    buffer_flag = 1; // 设置缓冲区轮转标志
                    break;
                }
            }
        }
        else if (c == '\n')
        {
            line_count++;
            double_buffer[current_buffer].buffer[double_buffer[current_buffer].count] = '\0';
        }
        else {
            double_buffer[current_buffer].buffer[double_buffer[current_buffer].count++] = c;
            continue;//继续
        }
        //继续处理换行后/缓冲池满后的处理
        deleComments(line_count);
        deleSpaces(line_count);

        if (double_buffer[current_buffer].count > 0)
        {
            strcpy(final_buffer.buffer, double_buffer[current_buffer].buffer);
            final_buffer.count = double_buffer[current_buffer].count;
            double_buffer[current_buffer].count = 0;
            if (!this->DFA(line_count)) {
                fclose(fcode);
                fclose(ftoken);
                fclose(ferror);
                this->error_line = line_count;
                return LEXICAL_ANALYSIS_ERROR;
            }
        }

        if (buffer_flag == 1)
        {
            //下一次 缓冲区轮转
            double_buffer[current_buffer].count = 0;
            current_buffer = 1 - current_buffer;
            buffer_flag = 0;
        }
    }
    fclose(fcode);
    fclose(ftoken);
    fclose(ferror);
    string t = "#";
    token_vec.push_back(token(t, 13, line_count));
    return SUCCESS;
}

void lexer::deleComments(int line_count) {
    char comment[BUFFER_SIZE];
    int comment_count = 0;
    bool flag_qoute = 0;
    //状态机 读到非“”包含的/进入循环
    for (int i = 0; double_buffer[current_buffer].buffer[i] != '\0'; i++)
    {
        if (double_buffer[current_buffer].buffer[i] == '"')
        {
            flag_qoute = 1 - flag_qoute;
            if (comment_flag != 1)
                continue;
        }
        if (flag_qoute == 1)
            if (comment_flag != 1)
                continue;
        if (double_buffer[current_buffer].buffer[i] == '/' || comment_flag == 1)
        {
            if (double_buffer[current_buffer].buffer[i + 1] == '\0')
            {
                continue;
            }
            if (double_buffer[current_buffer].buffer[i + 1] == '/' && !comment_flag)
            {
                //进入 //状态 直到\0停止
                int j;

                for (j = i; double_buffer[current_buffer].buffer[j] != '\0'; j++)
                {
                    comment[comment_count++] = double_buffer[current_buffer].buffer[j];
                    double_buffer[current_buffer].buffer[j] = '\0';
                }
                comment[comment_count] = '\0';
                fprintf(ftoken, "注释         %s     %d\n", comment, line_count);
                double_buffer[current_buffer].count -= comment_count;
                comment_count = 0;
                break;
            }
            if (double_buffer[current_buffer].buffer[i + 1] == '*' || comment_flag == 1)
            {
                //进入/* 状态 
                comment_flag = 1;
                int j;
                for (j = i + 2 * (1 - comment_flag); double_buffer[current_buffer].buffer[j] != '\0'; j++)
                {

                    comment[comment_count++] = double_buffer[current_buffer].buffer[j];
                    if (!flag_qoute && double_buffer[current_buffer].buffer[j] == '*' && double_buffer[current_buffer].buffer[j + 1] == '/')
                    {
                        comment_flag = 0;
                        comment[comment_count++] = '/';
                        comment[comment_count] = '\0';
                        fprintf(ftoken, "注释         %s    %d\n", comment, line_count);
                        double_buffer[current_buffer].count -= comment_count;
                        comment_count = 0;
                        break;
                    }
                }

                if (comment_flag == 0)
                    j = j + 2;

                //开始前移
                for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, i++)
                {
                    if (double_buffer[current_buffer].buffer[j] == '\n')
                    {
                        i--;
                        continue;
                    }
                    double_buffer[current_buffer].buffer[i] = double_buffer[current_buffer].buffer[j];
                }

                if (comment_flag) {
                    //意味着多行注释，直接printf
                    comment[comment_count] = '\0';
                    fprintf(ftoken, "注释         %s   %d\n", comment, line_count);
                    double_buffer[current_buffer].buffer[i] = '\0';
                    double_buffer[current_buffer].count -= comment_count;
                    break;
                }
                double_buffer[current_buffer].buffer[i] = '\0';
            }

        }
    }
}

void lexer::deleSpaces(int line_count)
{
    bool inString = true, inChar = true;
    for (int i = 0; double_buffer[current_buffer].buffer[i] != '\0'; i++)
    {
        char cur_char = double_buffer[current_buffer].buffer[i];
        //不能删除字符串内的空格
        if (cur_char == '"')
            inString = !inString;
        //不能删除空格字符
        if (cur_char == '\'')
            inChar = !inChar;
        if ((cur_char == ' ' || cur_char == '\t') && inString && inChar)
        {
            // 查找连续空格或制表符的结束位置
            int j = i + 1;
            for (; double_buffer[current_buffer].buffer[j] != '\0' && (double_buffer[current_buffer].buffer[j] == ' ' || double_buffer[current_buffer].buffer[j] == '\t'); j++)
            {
            }
            //如果空格或制表符在缓冲区末尾，则直接截断字符串
            if (double_buffer[current_buffer].buffer[j] == '\0')
            {
                double_buffer[current_buffer].buffer[i] = '\0';
                double_buffer[current_buffer].count -= (j - i);
                break;
            }
            // 如果空格或制表符在缓冲区开头，则全部删除
            if (i == 0)
            {
                int k = i;
                double_buffer[current_buffer].count -= (j - i);
                for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, k++)
                    double_buffer[current_buffer].buffer[k] = double_buffer[current_buffer].buffer[j];
                double_buffer[current_buffer].buffer[k] = '\0';
                i--; // 回退一个位置
            }
            else
            {
                //如果之间有多个空格，先删到只有一个
                if (j - i >= 2)
                {
                    int k = i + 1;
                    double_buffer[current_buffer].count -= (j - (i + 1));
                    for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, k++)
                        double_buffer[current_buffer].buffer[k] = double_buffer[current_buffer].buffer[j];
                    double_buffer[current_buffer].buffer[k] = '\0';
                    j = i + 1;
                }
                if (double_buffer[current_buffer].buffer[j] != '\0' && ((spaceRemovable(double_buffer[current_buffer].buffer[j]) ||
                    (i > 0 && spaceRemovable(double_buffer[current_buffer].buffer[i - 1])))))
                {
                    int k = i;
                    double_buffer[current_buffer].count -= (j - i);
                    for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, k++)
                        double_buffer[current_buffer].buffer[k] = double_buffer[current_buffer].buffer[j];
                    double_buffer[current_buffer].buffer[k] = '\0';
                    i--;
                }

            }
        }

    }

}

bool lexer::DFA(int line_count)
{
    char dfa_token[BUFFER_SIZE];
    int count = 0;//当前dfa_token中的字符个数
    bool finish = false;
    int state = 0;//初态，state为0就表示了在初态

    for (int i = 0; i <= final_buffer.count; i++)
    {
        switch (state)
        {
            //初态
            case 0:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1://字母
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 1;
                        break;
                    case 2://数字
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 2;
                        break;
                    case 3://$,_
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 3;
                        break;
                    case 4://转义符只会在字符串内部使用，否则作为一个字符单独出来
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 4;
                        break;
                    case 5:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 5;
                        break;
                    case 6:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 6;
                        break;
                    case 7:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 7;
                        break;
                    case 8:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 8;
                        break;
                    case 10:
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 10;
                        break;
                    default:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                }
                break;
            case 1:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1:case 2:case 3:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//结束状态
                }
                break;
            case 2:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1:
                    case 2:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    case 7:
                        if (final_buffer.buffer[i] == '.')
                        {
                            dfa_token[count++] = final_buffer.buffer[i];
                            break;
                        }
                        else
                        {
                            dfa_token[count] = '\0';
                            i--;
                            finish = 1;
                            state = 9;//结束状态
                        }
                        break;
                    case 8:
                        //现在是+-，前面是Ee
                        if ((final_buffer.buffer[i] == '+' || final_buffer.buffer[i] == '-') && (final_buffer.buffer[i - 1] == 'e' || final_buffer.buffer[i - 1] == 'E'))
                        {
                            dfa_token[count++] = final_buffer.buffer[i];
                            break;
                        }
                        else
                        {
                            dfa_token[count] = '\0';
                            i--;
                            finish = 1;
                            state = 9;//结束状态
                            break;
                        }
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//结束状态
                        break;
                }
                break;
            case 3://好像$_和字母是一样的操作
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1:case 2:case 3:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//结束状态
                        break;
                }
                break;
            case 4:
                //字符串内转义符的情况在5态内部处理，这里处理单独的'\'
                dfa_token[count] = '\0';
                i--;
                finish = 1;
                state = 9;//结束状态
                break;
            case 5:
                dfa_token[count++] = final_buffer.buffer[i];
                if (final_buffer.buffer[i] == '"')
                {
                    //此时一定不是初态，所以不需要判断i与1的关系
                    if (final_buffer.buffer[i - 1] == '\\')
                    {
                    }
                    else
                    {
                        dfa_token[count] = '\0';
                        finish = 1;
                        state = 9;
                    }
                }
                break;
            case 6:
                dfa_token[count++] = final_buffer.buffer[i];
                if (final_buffer.buffer[i] == '\'')
                {
                    //还有一种情况是'\''，还是得判断
                    if (final_buffer.buffer[i - 1] == '\\')
                    {
                    }
                    else
                    {
                        dfa_token[count] = '\0';
                        finish = 1;
                        state = 9;
                    }
                }
                break;
            case 7:
                //要结束的字符，直接结束
                dfa_token[count] = '\0';
                i--;
                finish = 1;
                state = 9;
                break;
            case 8:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 8:case 11:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;
                        break;
                }
                break;
            case 9://结束态
                //此时dfa_token已经得到，并且最后以\0结尾，故状态换成初始状态
                state = 0;
                count = 0;
                finish = 0;
                i--;
                if (!tokenJudge(dfa_token, line_count)) {
                    return false;
                }
                break;
            case 10://空格另加
                switch (toState(final_buffer.buffer[i]))
                {
                    case 10:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;
                        break;
                }
                break;

            default:
                break;
        }
        //最后剩的
        if (final_buffer.buffer[i + 1] == '\0')
        {
            dfa_token[count] = '\0';
            if (!tokenJudge(dfa_token, line_count)) {
                return false;
            }
            break;
        }
    }
   
    return true;
}

bool lexer::tokenJudge(char* str, int line_count)
{
    int kind = 0;
    if (isKeyWord(str) == 1) //判断是否为关键字
    {
        kind = my_Key_Word;
        fprintf(ftoken, "关键字       %-13s%d\n", str, line_count);
    }
    else if (isSignWord(str) == 1)  //判断是否为标识符
    {
        kind = my_SignWord;
        fprintf(ftoken, "标识符       %-13s%d\n", str, line_count);
    }
    else if (isInt(str) == 1)//判断是否为整数
    {
        kind = my_Integer;
        fprintf(ftoken, "整数         %-13s%d\n", str, line_count);
    }
    else if (isFloat(str) == 1)//判断是否为浮点数
    {
        kind = my_FloatPoint;
        fprintf(ftoken, "浮点数       %-13s%d\n", str, line_count);
    }
    else if (isMonocularOperator(str) == 1)//判断是否为单目运算符
    {
        kind = my_MonocularOperator;
        fprintf(ftoken, "单目运算符   %-13s%d\n", str, line_count);
    }
    else if (isBinocularOperator(str) == 1)//判断是否为双目运算符
    {
        kind = my_BinocularOperator;
        fprintf(ftoken, "双目运算符   %-13s%d\n", str, line_count);
    }
    else if (isDelimiter(str) == 1)//判断是否为界符
    {
        kind = my_Delimiter;
        fprintf(ftoken, "界符         %-13s%d\n", str, line_count);
    }
    else if (isBlank(str) == 1)//判断是否是空格
    {
        kind = my_Blank;
       // fprintf(ftoken, "空格         %-13s%d\n", str, line_count);
    }
    else if (isSeparator(str[0]) == 1 && strlen(str) == 1)//分隔符
    {
        kind = my_Separator;
        fprintf(ftoken, "分隔符       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeft(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsLeft;
        fprintf(ftoken, "左括号       %-13s%d\n", str, line_count);
    }
    else if (isBracketsRight(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsRight;
        fprintf(ftoken, "右括号       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeftBig(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsLeftBig;
        fprintf(ftoken, "左大括号     %-13s%d\n", str, line_count);
    }
    else if (isBracketsRightBig(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsRightBig;
        fprintf(ftoken, "右大括号     %-13s%d\n", str, line_count);
    }
    else if (isEnd(str[0]) == 1 && strlen(str) == 1) {
        kind = my_End;
        fprintf(ftoken, "结束符       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeftSquare(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Brackets_Left_Square;
        fprintf(ftoken, "左方括号     %-13s%d\n", str, line_count);
    }
    else if (isBracketsRightSquare(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Brackets_Right_Square;
        fprintf(ftoken, "右方括号     %-13s%d\n", str, line_count);
    }
    else if (isPointArrow(str) == 1) {
        kind = my_Point_Arrow;
        fprintf(ftoken, "箭头         %-13s%d\n", str, line_count);
    }
    else if (isRegion(str) == 1) {
        kind = my_Region;
        fprintf(ftoken, "域符         %-13s%d\n", str, line_count);
    }
    else if (isRegionDestroyer(str) == 1) {
        kind = my_Region_Destroyer;
        fprintf(ftoken, "析构符       %-13s%d\n", str, line_count);
    }
    else if (strlen(str) == 1 && isColon(str[0]) == 1) {
        kind = my_Colon;
        fprintf(ftoken, "冒号         %-13s%d\n", str, line_count);
    }
    else if (isPoint(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Point;
        fprintf(ftoken, "点符         %-13s%d\n", str, line_count);
    }
    else if (isStr(str)) {
        kind = my_Str;
        fprintf(ftoken, "字符串       %-13s%d\n", str, line_count);
    }
    else if (isChar(str)) {
        kind = my_Char;
        fprintf(ftoken, "字符         %-13s%d\n", str, line_count);
    }
    else if (strlen(str) == 1 && isQuestion_Mark(str[0]) == 1) {
        kind = my_Question_Mark;
        fprintf(ftoken, "问号         %-13s%d\n", str, line_count);
    }
    else {
        kind = my_WrongWord;
        fprintf(ftoken, "错误         %-13s%d\n", str, line_count);
        fprintf(ferror, "无法识别%s---------源代码第%d行\n", str, line_count);
    }
    if (kind != my_Blank)//送到语法分析器中的中间结果不含空格
    {
        token t = elemGenerate(kind, str, line_count);
        token_vec.push_back(t);
    }
    
    return (kind != my_WrongWord);
}

token lexer::elemGenerate(int kind, char* str, int line_count) {
    //判断是否是数据
    string word = str;
    //处理关键字
    if (kind == my_Key_Word)
    {
        token a(word, 0, line_count);
        return a;
    }
    //判断是否是标识符
    else if (kind == my_SignWord)
    {
        token a(word, 1, line_count);
        return a;
    }
    else if (kind == my_Integer || kind==my_FloatPoint)
    {
        token a(word, 2, line_count);
        return a;
    }
    else if (kind == my_MonocularOperator||kind==my_BinocularOperator)//算符
    {
        token a(word,3, line_count);
        return a;
    }
    //判断是否出错
    else if (kind == my_Delimiter)
    {
        token a(word, 4, line_count);
        return a;
    }
   
    //分隔符
    else if (kind == my_Separator)
    {
        token a(word, 5, line_count);
        return a;
    }
    else if (kind == my_BracketsLeft) {
        token a(word,7 , line_count);
        return a;
    }
    else if (kind == my_BracketsRight) {
        token a(word, 8, line_count);
        return a;
    }
    else if (kind == my_BracketsLeftBig) {
        token a(word, 9, line_count);
        return a;
    }
    else if (kind == my_BracketsRightBig) {
        token a(word, 10, line_count);
        return a;
    }
    else if (kind == my_Brackets_Left_Square) {
        token a(word, 11, line_count);
        return a;
    }
    else if (kind == my_Brackets_Right_Square) {
        token a(word, 12, line_count);
        return a;
    }
    else {
        token a(word, -1, line_count);
        return a;
    }
}

int toState(char c) {
    if (isalpha(c))
        return 1;
    else if (isdigit(c))
        return 2;
    else if (c == '$' || c == '_')
        return 3;
    else if (c == '\\')//转义问题
        return 4;
    else if (c == '"')
        return 5;
    else if (c == '\'')
        return 6;
    else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',' || c == '.')//这里需要注意.符号，若是在数字后面就需要考虑浮点数，这个单独考虑
        return 7;
    else if (c == ' ')
        return 10;
    return 8;
}

bool spaceRemovable(char c)
{
    if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<' || c == '!')
        return false;
    if ((c > 'z' || (c < 'a' && c > 'Z') || (c < 'A' && c > '9') || (c < '0')) && c != '_' && c != '$')
        return true;
    return false;
}

int isDelimiter(char c) {

    if (Delimiter == c)
        return 1;
    return 0;
}

int isDelimiter(string c) {
    if (c.length() == 1)
    {
        if (Delimiter == c[0])
            return 1;
    }
    return 0;
}

int isSignWord(string str) {

    if (str[0] == '_' || isalpha(str[0]) || str[0] == '$')
    {
        for (int i = 0; str[i] != '\0'; i++)
        {
            if (str[i] != '_' && str[i] != '$' && !isdigit(str[i]) && !isalpha(str[i]))
                return 2;
        }
        return 1;
    }
    return 0;
}

int isKeyWord(string str) {

    for (int i = 0; i < 50; i++)
    {
        if (str == Key_Word[i])
            return 1;
    }
    return 0;
}

int isBinocularOperator(string str) {
    for (int i = 0; i < BINOCULAR_OPERATOR_SIZE; i++)
    {
        if (str == Binocular_Operator[i])
            return 1;
    }
    return 0;
}

int isMonocularOperator(string str) {
    if (str.length() == 1) {
        for (int i = 0; i < MONOCULAR_OPERATOR_SIZE; i++)
        {
            if (str[0] == Monocular_Operator[i][0])
                return 1;
        }
    }
    return 0;
}

int isInt(string str) {
    const int OCT = 8;
    const int DEC = 10;
    const int HEX = 16;
    int intType = DEC;
    int len = 0;
    string t;
    if (str[0] == '+' || str[0] == '-')
        t = str.substr(1);
    else
        t = str;
    len = t.length();
    // 判断数的进制
    if (isdigit(t[0])) {
        if (t[0] == '0' && t[1] != '\0') {  //判断是八进制还是十六进制，当首位是0且数字不止一位时考虑下一位，
            if (isdigit(t[1]))
                intType = OCT;
            else if (t[1] == 'x' || t[1] == 'X')
                intType = HEX;
            else
                return 0;
        }
        else
            intType = DEC;
    }
    else
        return 0;
    //检查数的格式是否正确
    switch (intType)
    {
    case OCT:
        for (int i = 1; i < len; i++) {//八进制数首位为0，长度至少为2.
            if (isdigit(t[i])) {
                if (t[i] >= '0' && t[i] <= '7')
                    ;
                else
                    return 2;
            }
            else
                return 0;
        }
        return 1;
        break;
    case DEC:
        for (int i = 0; i < len; i++) {
            if (isdigit(t[i]))
                ;
            else
                return 0;
        }
        return 1;
        break;
    case HEX:
        for (int i = 2; i < len; i++) {  // 十六进制数前两位为0x\0X，长度至少为3.
            if (isdigit(t[i]) || (t[i] >= 'a' && t[i] <= 'f') || (t[i] >= 'A' && t[i] <= 'F'))
                ;
            else {
                if (!isalpha(t[i]))//十六进制数中出现非字母
                    return 0;
                else
                    return 2;
            }
        }
        return 1;
        break;
    }
    return 0;
}

//判断xx.xx的情况，无小数点视为非小数，否则无法与整数区分
int hasDot(string str)
{
    int len = 0;
    int dotAppearNum = 0;
    string t;
    if (str[0] == '+' || str[0] == '-')
        t = str.substr(1);
    else
        t = str;
    len = t.length();
    for (int i = 0; i < len; i++)
    {
        if (len > 0)//确保有首位和最后一位
        {
            if (i == 0)//判断首位
            {
                if (!isdigit(t[i]))//不是数字
                {
                    if (t[i] == '.')
                        return 2;
                    else
                        return 0;
                }
            }
            if (i < len - 1)//判断非最后一位且不是首位
            {
                if (!isdigit(t[i]) && t[i] != '.')//既不是数字也不是小数点
                    return 0;
                if (t[i] == '.')//如果是小数点
                {
                    dotAppearNum++;
                    if (dotAppearNum > 1)//出现多个小数点的情况
                        return 2;
                }
            }
            if (i == len - 1)//判断末位
            {
                //既不是数字也不是f\F、d\D
                if (!isdigit(t[i]) && t[i] != 'f' && t[i] != 'F' && t[i] != 'd' && t[i] != 'D')
                    return 0;
            }
        }
    }
    if (dotAppearNum < 1)
        return 0;
    return 1;
}

int isFloat(string str) {
    int len = 0;
    int dotAppearNum = 0;
    len = str.length();
    string t;
    //判断78e\E56的情况
    int pos = -1;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == 'e' || str[i] == 'E')
        {
            pos = i;
            break;
        }
    }
    if (pos != -1)//出现了e/E
    {
        int j = 0;
        if (pos == 0 || pos == len - 1)//e在首位或末尾
            return 0;
        if (str[0] == '+' || str[0] == '-')
        {
            if (str[1] == 'e' || str[1] == 'E')//首位是符号位时保证e不在符号位之后
                return 0;
        }
        t = str.substr(j, pos + 1);//最后一位不是尾零
        t[pos] = '\0';
        if (hasDot(t) != 1 && isInt(t) != 1)
            return 0;
        j = pos + 1;//数字的起始位        
        t = str.substr(j, len - pos);//最后一位是尾零
        if (hasDot(t) != 1 && isInt(t) != 1)
            return 0;
        return 1;
    }
    return hasDot(str);
}

int isBlank(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
            return 0;
    }
    return 1;
}

int isSeparator(char c)
{
    if (Separator == c)
        return 1;
    return 0;
}

int isBracketsLeft(char c)
{
    if (Brackets_Left == c)
        return 1;
    return 0;
}

int isBracketsRight(char c)
{
    if (Brackets_Right == c)
        return 1;
    return 0;
}

int isBracketsLeftBig(char c)
{
    if (Brackets_Left_Big == c)
        return 1;
    return 0;
}

int isBracketsRightBig(char c)
{
    if (Brackets_Right_Big == c)
        return 1;
    return 0;
}

int isPoint(char c)
{
    if (Point == c)
        return 1;
    return 0;
}

int isEnd(char c)
{
    if (End == c)
        return 1;
    return 0;
}

int isStr(string str)
{
    //小型状态机
    int i = 0;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
            continue;
        else
            break;
    }
    int flag = 0;
    //此时i为非' '的首字符
    if (str[i] == '"')
    {
        i++;
        flag = 1;//第一个"
    }
    if (flag == 1)
    {
        for (i; i < str.length(); i++)
        {
            if (str[i] == '"' && str[i - 1] != '\\')
                return 1;
        }
    }
    return 0;
}

int isChar(string str)
{
    //小型状态机
    int i = 0;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
            continue;
        else
            break;
    }
    int flag = 0;
    //此时i为非' '的首字符
    if (str[i] == '\'')
    {
        i++;
        flag = 1;//第一个"
    }
    if (flag == 1)
    {
        //只允许 '\x'
        if (str[i] == '\\')
        {
            if (isdigit(str[i]))
            {
                flag = 2;//10进制
            }
            else if (str[i] == 'x')
            {
                flag = 3;//16进制
            }
            else
            {
                if (str[i + 2] == '\'')
                    return 1;

            }
            if (flag == 2 && isInt(&str[i]) && strlen(&str[i]) == 3)
                return 1;
            if (flag == 3 && isInt(&str[i]) && strlen(&str[i]) == 5)
                return 1;

        }
        else {
            if (str[i + 1] == '\'')
                return 1;
            return 0;
        }

    }
    return 0;
}

int isBracketsLeftSquare(char c)
{
    if (Brackets_Left_Square == c)
        return 1;
    return 0;
}

int isBracketsRightSquare(char c)
{
    if (Brackets_Right_Square == c)
        return 1;
    return 0;
}

int isRegion(string str)
{
    if (str == Region)
        return 1;
    return 0;
}

int isRegionDestroyer(string str)
{
    if (str == Region_Destroyer)
        return 1;
    return 0;
}

int isColon(char c)
{
    if (c == Colon)
        return 1;
    return 0;
}

int isQuestion_Mark(char c) {
    if (c == Question_Mark)
        return 1;
    return 0;
}

int isPointArrow(string str)
{
    if (str == Point_Arrow)
        return 1;
    return 0;
}



