import os
import re
import LexAnalyzer
import SyntaxAnalyzer

folder = "./PROCESS_FILES"
LexOutputFilename0 = "0.LexSynCheckList.txt"
LexOutputFilename1 = "1.LexResultIndex.txt"
LexOutputFilename2 = "2.LexResultList.txt"
LexOutputFilename3 = "3.LexResultWord.txt"
LexOutputFilename3_display = "LexResultWord_display.txt"
LexOutputFilename4 = "4.SyntaxGrammar.txt"
LexOutputFilename4_display = "SyntaxGrammar_display.txt"

src = ""
lex = ""
show_highlights = True


def boundary_print(prompt):
    indent = 1
    print('-' * (len(prompt) * 3 + 2 * indent))
    print('-' * len(prompt), end='')
    print(' ' * indent + prompt + ' ' * indent, end='')
    print('-' * len(prompt))
    print('-' * (len(prompt) * 3 + 2 * indent))
    print(f"Symbol\t\tType")
    print('-' * (len(prompt) * 1 + 2 * indent))

def Lex_main(src):
    # -------------------------------LEXICAL AYA-------------------------------------
    # is_Lex_analyze_print = True
    is_Lex_analyze_print = False

    if is_Lex_analyze_print:    
        boundary_print("Lexical Analysis Start")
        
    try:
        output1 = open((folder + "/" + LexOutputFilename1), "w")
        output2 = open((folder + "/" + LexOutputFilename2), "w")
        output3 = open((folder + "/" + LexOutputFilename3), "w")
        output3_display = open((folder + "/" + LexOutputFilename3_display), "w")
    except:
        print(f"Fail to open {LexOutputFilename1}")
        return -1

    analyzer = LexAnalyzer.LexAnalyzer()
    analyzer.Analyze(src)
    res = analyzer.Get_Result()
    res.sort(key=lambda x: x.li)
    for seg in res:
        if seg.ctype != LexAnalyzer.VT.VTWhiteSpace and seg.ctype != LexAnalyzer.VT.VTEndLine:
            output1.write(f"{seg.li} {seg.ri} {seg.ctype.value}\n")
            if seg.ctype != LexAnalyzer.VT.VTComment and seg.ctype != LexAnalyzer.VT.VTMacro:
                output2.write(f"{seg.ctype.value}\n")
            output3.write(f"{src[seg.li:seg.ri]} {LexAnalyzer.VT_to_string_(seg.ctype)}\n")
            # output3_display.write(f"{src[seg.li:seg.ri]} ----> {LexAnalyzer.VT_to_string_(seg.ctype)}\n")

            if is_Lex_analyze_print:
                print(f"{src[seg.li:seg.ri]}\t\t{LexAnalyzer.VT_to_type_(seg.ctype)}")

    output2.write(f"{-1}\n")
    output1.close()
    output2.close()
    output3.close()

    # ---------------------------GRAMMAR---------------------------------
    segs = []
    with open(os.path.join(folder, LexOutputFilename1), 'r') as lex_index_file:
        for line in lex_index_file:
            left_index, right_index, comp = list(map(int, line.split(' ')))
            if comp != LexAnalyzer.VT.VTWhiteSpace and comp != LexAnalyzer.VT.VTEndLine:
                segs.append(LexAnalyzer.LexSegment(left_index, right_index, LexAnalyzer.VT(comp)))
    segs.sort(key=lambda x: x.li)  # no overlap zones (free from bothering left/right order)

    # for i in range(len(segs)):
    #     print(f"left: {segs[i].li}\tright: {segs[i].ri}\tcomp: {segs[i].ctype.value}")

    SynTax = SyntaxAnalyzer.SyntaxAnalyzer()
    SyntaxGrammarFile = open((folder + "/" + LexOutputFilename4), "w")
    SyntaxGrammarFile_display = open((folder + "/" + LexOutputFilename4_display), "w")
    SynTax.LR1_Analyze_file(SyntaxGrammarFile, SyntaxGrammarFile_display)
    SyntaxGrammarFile.close()
    SyntaxGrammarFile_display.close()

    with open((folder + "/" + LexOutputFilename0), "w") as LexSynCheckListFile:
        for i in range(0, LexAnalyzer.VT_MAX - LexAnalyzer.VT_MIN + 1):
            LexSynCheckListFile.write(f"{LexAnalyzer.VT_to_string(i)} {i + LexAnalyzer.VT_MIN}\n")

        for i in range(0, SyntaxAnalyzer.VN_MAX - SyntaxAnalyzer.VN_MIN + 1):
            LexSynCheckListFile.write(f"{SyntaxAnalyzer.VN_to_string(i)} {i + SyntaxAnalyzer.VN_MIN}\n")
        
def read_file_as_string(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()
    return content

def lex_exe(fileContent):
    Lex_main(fileContent + " ")

if __name__ == "__main__":
    src = read_file_as_string("test2.cpp") + " "
    Lex_main(src)
