#pragma execution_character_set("utf-8") 
#include"LexicalAnalysis.h"
#include<sstream>

token::token(string w, int t, int l)
{
    this->word = w;//����
    this->type = t;//ֵ
    this->line = l;//��������
}

int lexer::lexicalAnalysis(string file_name) {

    if ((this->fcode = fopen(file_name.c_str(), "r")) == NULL) {
        return  CODE_FILE_OPEN_ERROR;
    }

    this->ftoken = fopen(TOKEN_LIST_FILE, "w");//�洢�ʷ��������
    fprintf(this->ftoken, "Token����    Tokenֵ      ��������\n");
    this->ferror = fopen(ERROR_LIST_FILE, "w");

    char c = '\0';
    int buffer_flag = 0;//�������Ƿ���Ҫ��ת
    int line_count = 0;//��¼��ǰ����

    while (1)
    {
        c = fgetc(this->fcode);
        if (c == EOF)//�����ȡ����
        {
            line_count++;
            this->deleComments(line_count);// ɾ��ע��
            this->deleSpaces(line_count); // ɾ������ո�
            if (this->double_buffer[this->current_buffer].count > 0)
            {
                strcpy(this->final_buffer.buffer, this->double_buffer[this->current_buffer].buffer);
                this->final_buffer.count = this->double_buffer[this->current_buffer].count;
                //����״̬������ 
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
        //���������
        if (this->double_buffer[this->current_buffer].count == BUFFER_SIZE - 2)
        {
            this->double_buffer[this->current_buffer].buffer[this->double_buffer[this->current_buffer].count] = c;
            int i;
            for (i = 0; i < this->double_buffer[this->current_buffer].count; i++)
            {
                if (isDelimiter(double_buffer[this->current_buffer].buffer[i]))
                {
                    int j;//�ֽ��
                    int k;
                    // ת�ƻ���������
                    for (j = 0, k = i + 1; k <= double_buffer[current_buffer].count; k++, j++)
                    {
                        double_buffer[1 - current_buffer].buffer[j] = double_buffer[current_buffer].buffer[k];
                    }
                    //count��С��������
                    double_buffer[1 - current_buffer].count = j;
                    double_buffer[current_buffer].count = i;
                    //�����ս��
                    double_buffer[1 - current_buffer].buffer[j] = '\0';
                    double_buffer[current_buffer].buffer[i + 1] = '\0';
                    buffer_flag = 1; // ���û�������ת��־
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
            continue;//����
        }
        //���������к�/���������Ĵ���
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
            //��һ�� ��������ת
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
    //״̬�� �����ǡ���������/����ѭ��
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
                //���� //״̬ ֱ��\0ֹͣ
                int j;

                for (j = i; double_buffer[current_buffer].buffer[j] != '\0'; j++)
                {
                    comment[comment_count++] = double_buffer[current_buffer].buffer[j];
                    double_buffer[current_buffer].buffer[j] = '\0';
                }
                comment[comment_count] = '\0';
                fprintf(ftoken, "ע��         %s     %d\n", comment, line_count);
                double_buffer[current_buffer].count -= comment_count;
                comment_count = 0;
                break;
            }
            if (double_buffer[current_buffer].buffer[i + 1] == '*' || comment_flag == 1)
            {
                //����/* ״̬ 
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
                        fprintf(ftoken, "ע��         %s    %d\n", comment, line_count);
                        double_buffer[current_buffer].count -= comment_count;
                        comment_count = 0;
                        break;
                    }
                }

                if (comment_flag == 0)
                    j = j + 2;

                //��ʼǰ��
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
                    //��ζ�Ŷ���ע�ͣ�ֱ��printf
                    comment[comment_count] = '\0';
                    fprintf(ftoken, "ע��         %s   %d\n", comment, line_count);
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
        //����ɾ���ַ����ڵĿո�
        if (cur_char == '"')
            inString = !inString;
        //����ɾ���ո��ַ�
        if (cur_char == '\'')
            inChar = !inChar;
        if ((cur_char == ' ' || cur_char == '\t') && inString && inChar)
        {
            // ���������ո���Ʊ���Ľ���λ��
            int j = i + 1;
            for (; double_buffer[current_buffer].buffer[j] != '\0' && (double_buffer[current_buffer].buffer[j] == ' ' || double_buffer[current_buffer].buffer[j] == '\t'); j++)
            {
            }
            //����ո���Ʊ���ڻ�����ĩβ����ֱ�ӽض��ַ���
            if (double_buffer[current_buffer].buffer[j] == '\0')
            {
                double_buffer[current_buffer].buffer[i] = '\0';
                double_buffer[current_buffer].count -= (j - i);
                break;
            }
            // ����ո���Ʊ���ڻ�������ͷ����ȫ��ɾ��
            if (i == 0)
            {
                int k = i;
                double_buffer[current_buffer].count -= (j - i);
                for (; double_buffer[current_buffer].buffer[j] != '\0'; j++, k++)
                    double_buffer[current_buffer].buffer[k] = double_buffer[current_buffer].buffer[j];
                double_buffer[current_buffer].buffer[k] = '\0';
                i--; // ����һ��λ��
            }
            else
            {
                //���֮���ж���ո���ɾ��ֻ��һ��
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
    int count = 0;//��ǰdfa_token�е��ַ�����
    bool finish = false;
    int state = 0;//��̬��stateΪ0�ͱ�ʾ���ڳ�̬

    for (int i = 0; i <= final_buffer.count; i++)
    {
        switch (state)
        {
            //��̬
            case 0:
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1://��ĸ
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 1;
                        break;
                    case 2://����
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 2;
                        break;
                    case 3://$,_
                        dfa_token[count++] = final_buffer.buffer[i];
                        state = 3;
                        break;
                    case 4://ת���ֻ�����ַ����ڲ�ʹ�ã�������Ϊһ���ַ���������
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
                        state = 9;//����״̬
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
                            state = 9;//����״̬
                        }
                        break;
                    case 8:
                        //������+-��ǰ����Ee
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
                            state = 9;//����״̬
                            break;
                        }
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//����״̬
                        break;
                }
                break;
            case 3://����$_����ĸ��һ���Ĳ���
                switch (toState(final_buffer.buffer[i]))
                {
                    case 1:case 2:case 3:
                        dfa_token[count++] = final_buffer.buffer[i];
                        break;
                    default:
                        dfa_token[count] = '\0';
                        i--;
                        finish = 1;
                        state = 9;//����״̬
                        break;
                }
                break;
            case 4:
                //�ַ�����ת����������5̬�ڲ��������ﴦ������'\'
                dfa_token[count] = '\0';
                i--;
                finish = 1;
                state = 9;//����״̬
                break;
            case 5:
                dfa_token[count++] = final_buffer.buffer[i];
                if (final_buffer.buffer[i] == '"')
                {
                    //��ʱһ�����ǳ�̬�����Բ���Ҫ�ж�i��1�Ĺ�ϵ
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
                    //����һ�������'\''�����ǵ��ж�
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
                //Ҫ�������ַ���ֱ�ӽ���
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
            case 9://����̬
                //��ʱdfa_token�Ѿ��õ������������\0��β����״̬���ɳ�ʼ״̬
                state = 0;
                count = 0;
                finish = 0;
                i--;
                if (!tokenJudge(dfa_token, line_count)) {
                    return false;
                }
                break;
            case 10://�ո����
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
        //���ʣ��
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
    if (isKeyWord(str) == 1) //�ж��Ƿ�Ϊ�ؼ���
    {
        kind = my_Key_Word;
        fprintf(ftoken, "�ؼ���       %-13s%d\n", str, line_count);
    }
    else if (isSignWord(str) == 1)  //�ж��Ƿ�Ϊ��ʶ��
    {
        kind = my_SignWord;
        fprintf(ftoken, "��ʶ��       %-13s%d\n", str, line_count);
    }
    else if (isInt(str) == 1)//�ж��Ƿ�Ϊ����
    {
        kind = my_Integer;
        fprintf(ftoken, "����         %-13s%d\n", str, line_count);
    }
    else if (isFloat(str) == 1)//�ж��Ƿ�Ϊ������
    {
        kind = my_FloatPoint;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isMonocularOperator(str) == 1)//�ж��Ƿ�Ϊ��Ŀ�����
    {
        kind = my_MonocularOperator;
        fprintf(ftoken, "��Ŀ�����   %-13s%d\n", str, line_count);
    }
    else if (isBinocularOperator(str) == 1)//�ж��Ƿ�Ϊ˫Ŀ�����
    {
        kind = my_BinocularOperator;
        fprintf(ftoken, "˫Ŀ�����   %-13s%d\n", str, line_count);
    }
    else if (isDelimiter(str) == 1)//�ж��Ƿ�Ϊ���
    {
        kind = my_Delimiter;
        fprintf(ftoken, "���         %-13s%d\n", str, line_count);
    }
    else if (isBlank(str) == 1)//�ж��Ƿ��ǿո�
    {
        kind = my_Blank;
       // fprintf(ftoken, "�ո�         %-13s%d\n", str, line_count);
    }
    else if (isSeparator(str[0]) == 1 && strlen(str) == 1)//�ָ���
    {
        kind = my_Separator;
        fprintf(ftoken, "�ָ���       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeft(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsLeft;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isBracketsRight(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsRight;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeftBig(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsLeftBig;
        fprintf(ftoken, "�������     %-13s%d\n", str, line_count);
    }
    else if (isBracketsRightBig(str[0]) == 1 && strlen(str) == 1) {
        kind = my_BracketsRightBig;
        fprintf(ftoken, "�Ҵ�����     %-13s%d\n", str, line_count);
    }
    else if (isEnd(str[0]) == 1 && strlen(str) == 1) {
        kind = my_End;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (isBracketsLeftSquare(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Brackets_Left_Square;
        fprintf(ftoken, "������     %-13s%d\n", str, line_count);
    }
    else if (isBracketsRightSquare(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Brackets_Right_Square;
        fprintf(ftoken, "�ҷ�����     %-13s%d\n", str, line_count);
    }
    else if (isPointArrow(str) == 1) {
        kind = my_Point_Arrow;
        fprintf(ftoken, "��ͷ         %-13s%d\n", str, line_count);
    }
    else if (isRegion(str) == 1) {
        kind = my_Region;
        fprintf(ftoken, "���         %-13s%d\n", str, line_count);
    }
    else if (isRegionDestroyer(str) == 1) {
        kind = my_Region_Destroyer;
        fprintf(ftoken, "������       %-13s%d\n", str, line_count);
    }
    else if (strlen(str) == 1 && isColon(str[0]) == 1) {
        kind = my_Colon;
        fprintf(ftoken, "ð��         %-13s%d\n", str, line_count);
    }
    else if (isPoint(str[0]) == 1 && strlen(str) == 1) {
        kind = my_Point;
        fprintf(ftoken, "���         %-13s%d\n", str, line_count);
    }
    else if (isStr(str)) {
        kind = my_Str;
        fprintf(ftoken, "�ַ���       %-13s%d\n", str, line_count);
    }
    else if (isChar(str)) {
        kind = my_Char;
        fprintf(ftoken, "�ַ�         %-13s%d\n", str, line_count);
    }
    else if (strlen(str) == 1 && isQuestion_Mark(str[0]) == 1) {
        kind = my_Question_Mark;
        fprintf(ftoken, "�ʺ�         %-13s%d\n", str, line_count);
    }
    else {
        kind = my_WrongWord;
        fprintf(ftoken, "����         %-13s%d\n", str, line_count);
        fprintf(ferror, "�޷�ʶ��%s---------Դ�����%d��\n", str, line_count);
    }
    if (kind != my_Blank)//�͵��﷨�������е��м��������ո�
    {
        token t = elemGenerate(kind, str, line_count);
        token_vec.push_back(t);
    }
    
    return (kind != my_WrongWord);
}

token lexer::elemGenerate(int kind, char* str, int line_count) {
    //�ж��Ƿ�������
    string word = str;
    //����ؼ���
    if (kind == my_Key_Word)
    {
        token a(word, 0, line_count);
        return a;
    }
    //�ж��Ƿ��Ǳ�ʶ��
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
    else if (kind == my_MonocularOperator||kind==my_BinocularOperator)//���
    {
        token a(word,3, line_count);
        return a;
    }
    //�ж��Ƿ����
    else if (kind == my_Delimiter)
    {
        token a(word, 4, line_count);
        return a;
    }
   
    //�ָ���
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
    else if (c == '\\')//ת������
        return 4;
    else if (c == '"')
        return 5;
    else if (c == '\'')
        return 6;
    else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',' || c == '.')//������Ҫע��.���ţ����������ֺ������Ҫ���Ǹ������������������
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
    // �ж����Ľ���
    if (isdigit(t[0])) {
        if (t[0] == '0' && t[1] != '\0') {  //�ж��ǰ˽��ƻ���ʮ�����ƣ�����λ��0�����ֲ�ֹһλʱ������һλ��
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
    //������ĸ�ʽ�Ƿ���ȷ
    switch (intType)
    {
    case OCT:
        for (int i = 1; i < len; i++) {//�˽�������λΪ0����������Ϊ2.
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
        for (int i = 2; i < len; i++) {  // ʮ��������ǰ��λΪ0x\0X����������Ϊ3.
            if (isdigit(t[i]) || (t[i] >= 'a' && t[i] <= 'f') || (t[i] >= 'A' && t[i] <= 'F'))
                ;
            else {
                if (!isalpha(t[i]))//ʮ���������г��ַ���ĸ
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

//�ж�xx.xx���������С������Ϊ��С���������޷�����������
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
        if (len > 0)//ȷ������λ�����һλ
        {
            if (i == 0)//�ж���λ
            {
                if (!isdigit(t[i]))//��������
                {
                    if (t[i] == '.')
                        return 2;
                    else
                        return 0;
                }
            }
            if (i < len - 1)//�жϷ����һλ�Ҳ�����λ
            {
                if (!isdigit(t[i]) && t[i] != '.')//�Ȳ�������Ҳ����С����
                    return 0;
                if (t[i] == '.')//�����С����
                {
                    dotAppearNum++;
                    if (dotAppearNum > 1)//���ֶ��С��������
                        return 2;
                }
            }
            if (i == len - 1)//�ж�ĩλ
            {
                //�Ȳ�������Ҳ����f\F��d\D
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
    //�ж�78e\E56�����
    int pos = -1;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == 'e' || str[i] == 'E')
        {
            pos = i;
            break;
        }
    }
    if (pos != -1)//������e/E
    {
        int j = 0;
        if (pos == 0 || pos == len - 1)//e����λ��ĩβ
            return 0;
        if (str[0] == '+' || str[0] == '-')
        {
            if (str[1] == 'e' || str[1] == 'E')//��λ�Ƿ���λʱ��֤e���ڷ���λ֮��
                return 0;
        }
        t = str.substr(j, pos + 1);//���һλ����β��
        t[pos] = '\0';
        if (hasDot(t) != 1 && isInt(t) != 1)
            return 0;
        j = pos + 1;//���ֵ���ʼλ        
        t = str.substr(j, len - pos);//���һλ��β��
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
    //С��״̬��
    int i = 0;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
            continue;
        else
            break;
    }
    int flag = 0;
    //��ʱiΪ��' '�����ַ�
    if (str[i] == '"')
    {
        i++;
        flag = 1;//��һ��"
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
    //С��״̬��
    int i = 0;
    for (i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
            continue;
        else
            break;
    }
    int flag = 0;
    //��ʱiΪ��' '�����ַ�
    if (str[i] == '\'')
    {
        i++;
        flag = 1;//��һ��"
    }
    if (flag == 1)
    {
        //ֻ���� '\x'
        if (str[i] == '\\')
        {
            if (isdigit(str[i]))
            {
                flag = 2;//10����
            }
            else if (str[i] == 'x')
            {
                flag = 3;//16����
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



