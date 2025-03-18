import tkinter as tk
from tkinter import *
from tkinter import ttk
from tkinter import filedialog
from tkinter import PhotoImage
from PIL import Image, ImageTk
import re
from tkinter import filedialog
from pygments import highlight
from pygments.lexers import get_lexer_by_name
from pygments.formatters import get_formatter_by_name
from Lex_main import *
from Syn_main import *

VT_names = [
    "Null",
    "Integer",
    "String",
    "Boolean",
    "Floating",
    "Character",
    "Comment",
    "Macro",
    "WhiteSpace",
    "EndLine",
    # 运算符
    "{",
    "}",
    "[",
    "]",
    "(",
    ")",
    "+",
    "-",
    "*",
    "/",
    "%",
    "++",
    "--",
    "==",
    "!=",
    ">",
    "<",
    ">=",
    "<=",
    "&&",
    "||",
    "!",
    "&",
    "|",
    "~",
    "^",
    "<<",
    ">>",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "<<=",
    ">>=",
    "&=",
    "|=",
    "^=",
    ",",
    ";",
    # 关键字
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
    "Identifier",
    "Word"
]

VN_names=["emptypro",
    "A",
    "P",
    "M",
    "N",
    "add_expression",
    "argument_list",
    "assign_sentence",
    "declare",
    "declare_list",
    "expression",
    "factor",
    "function_declare",
    "if_sentence",
    "inner_declare",
    "inner_var_declare",
    "item",
    "param",
    "parameter",
    "parameter_list",
    "return_sentence",
    "sentence",
    "sentence_block",
    "sentence_list",
    "var_declare",
    "while_sentence"]

V_ALL = VT_names + VN_names

welcome_string = """
                                                                               
                                                                               
           .---.            ,--,                                ____           
          /. ./|          ,--.'|                              ,'  , `.         
      .--'.  ' ;          |  | :               ,---.       ,-+-,.' _ |         
     /__./ \ : |          :  : '              '   ,'\   ,-+-. ;   , ||         
 .--'.  '   \' .   ,---.  |  ' |      ,---.  /   /   | ,--.'|'   |  || ,---.   
/___/ \ |    ' '  /     \ '  | |     /     \.   ; ,. :|   |  ,', |  |,/     \  
;   \  \;      : /    /  ||  | :    /    / ''   | |: :|   | /  | |--'/    /  | 
 \   ;  `      |.    ' / |'  : |__ .    ' / '   | .; :|   : |  | ,  .    ' / | 
  .   \    .\  ;'   ;   /||  | '.'|'   ; :__|   :    ||   : |  |/   '   ;   /| 
   \   \   ' \ |'   |  / |;  :    ;'   | '.'|\   \  / |   | |`-'    '   |  / | 
    :   '  |--" |   :    ||  ,   / |   :    : `----'  |   ;/        |   :    | 
     \   \ ;     \   \  /  ---`-'   \   \  /          '---'          \   \  /  
      '---"       `----'             `----'                           `----'   





                          Lexical & Syntax Analyzer
                                 11/28/2023
                                                                               
"""

def configure_tags(text_widget, tags):
    for tag, color in tags.items():
        text_widget.tag_delete(tag)
        text_widget.tag_config(tag, foreground=color)


def on_key_release(text_widget):
    lines = text_widget.get(1.0, tk.END).splitlines()
    regex = re.compile(
        r"(^\s*"
        r"(?P<if>if)" + "|"  # if condition
        r"(?P<for>for)" + "|"  # for loop
        r"(?P<include>#include\s+[\"<]\S+)" + "|"
        r"(?P<int>int)"  # variable
        r"[\s\(]+)"
    )
    for idx, line in enumerate(lines):
        int_tag = f"int_{idx}"
        for_tag = f"for_{idx}"
        if_tag = f"if_{idx}"
        include_tag = f"include_{idx}"
        tags = {
            int_tag: "blue",
            for_tag: "green",
            if_tag: "purple",
            include_tag: "green",
            # add new tag here
        }
        configure_tags(text_widget, tags)

        for match in regex.finditer(line):
            for tag in tags:
                group_name = tag.split("_")[0]
                if -1 != match.start(group_name):
                    text_widget.tag_add(
                        tag,
                        "{0}.{1}".format(idx+1, match.start(group_name)),
                        "{0}.{1}".format(idx+1, match.end(group_name))
                    )


class FileAnalyzerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("词法分析器与语法分析器")
        self.root.geometry("600x400")  # 设置窗口大小

        self.file_content = tk.StringVar()

        # 文本展示框
        self.text_display = tk.Text(root, height=25, width=85)
        self.text_display.grid(row=0, column=0, columnspan=3, pady=10)
        self.text_display.insert(tk.END, welcome_string)  # 初始文本

        # 文件上传按钮
        self.file_upload_button = tk.Button(root, text="文件上传", command=self.upload_file, width=15)
        self.file_upload_button.grid(row=1, column=0, pady=10)

        # 词法分析按钮
        self.lexical_analysis_button = tk.Button(root, text="词法分析", command=self.lexical_analysis, width=15, state="disabled")
        self.lexical_analysis_button.grid(row=1, column=1, pady=10)
        self.lexical_analysis_button.pack_forget()

        # 语法分析按钮
        self.syntax_analysis_button = tk.Button(root, text="语法分析", command=self.syntax_analysis, width=15, state="disabled")
        self.syntax_analysis_button.grid(row=1, column=2, pady=10)
        self.syntax_analysis_button.pack_forget()

    def show_analysis_buttons(self):
        # 显示词法分析和语法分析按钮
        self.lexical_analysis_button["state"] = "normal"
        
    def upload_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("All files", "*.*")])
        if file_path:
            with open(file_path, "r") as file:
                content = file.read()
                self.file_content.set(content)
                self.text_display.delete(1.0, tk.END)
                on_key_release(self.text_display)
                self.text_display.bind("<Motion>", lambda event: on_key_release(self.text_display))
                self.text_display.insert(tk.END, content)
            self.show_analysis_buttons()

    def lexical_analysis(self):
        print("词法分析执行中...")
        text_content = self.text_display.get("1.0", tk.END)
        lex_exe(text_content)
        print("词法分析执行完毕...")
        self.open_lexical_window()
        self.syntax_analysis_button["state"] = "normal"
    
    def open_lexical_window(self):
        lexical_window = tk.Toplevel(self.root)
        lexical_window.title("Lexical Analysis Results")
        

        # 创建Treeview表格
        tree = ttk.Treeview(lexical_window)
        tree["columns"] = ("Token", "Type")
        tree.column("#0", width=0, stretch=tk.NO)  # 隐藏行号列
        tree.column("Token", anchor=tk.W, width=200)
        tree.column("Type", anchor=tk.W, width=200)

        # 设置表头
        tree.heading("#0", text="", anchor=tk.W)
        tree.heading("Token", text="Token", anchor=tk.W)
        tree.heading("Type", text="Type", anchor=tk.W)

        # 从文件中读取内容并添加到表格中
        file_path = "./PROCESS_FILES/3.LexResultWord.txt"  # 替换为实际文件路径
        with open(file_path, "r", encoding="utf-8") as file:
            for line in file:
                tokens = line.split()
                tree.insert("", "end", values=(tokens[0], tokens[1]))

        tree.pack(padx=10, pady=10)
        
    def syntax_analysis(self):
        print("语法分析执行中...")
        status = syn_exe()
        if status == 0:
            print("语法分析执行完毕...")
            self.open_syntax_window()
        elif status == -1:
            popup = tk.Toplevel(self.root)
            popup.title("ERROR!")

            label = tk.Label(popup, text="规约过程出错！")
            label.pack(padx=10, pady=10)

            close_button = tk.Button(popup, text="Close", command=popup.destroy)
            close_button.pack(pady=10)
        else:
            popup = tk.Toplevel(self.root)
            popup.title("ERROR!")

            label = tk.Label(popup, text="该文法不是LR(1)文法！")
            label.pack(padx=10, pady=10)

            close_button = tk.Button(popup, text="Close", command=popup.destroy)
            close_button.pack(pady=10)
        
    def open_syntax_window(self):
        syntax_window = tk.Toplevel(self.root)
        syntax_window.title("终结符、非终结符和语法")
        syntax_window.geometry("850x700")  # 设置窗口大小

        # 两个列表格
        self.tree1 = ttk.Treeview(syntax_window)
        self.tree1["columns"] = ("Terminals", "Index")
        self.tree1.column("#0", width=0, stretch=tk.NO)
        self.tree1.column("Terminals", anchor=tk.W, width=200)
        self.tree1.column("Index", anchor=tk.W, width=200)
        self.tree1.heading("#0", text="", anchor=tk.W)
        self.tree1.heading("Terminals", text="Terminals", anchor=tk.W)
        self.tree1.heading("Index", text="Index", anchor=tk.W)
        self.tree1.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")
        

        self.tree2 = ttk.Treeview(syntax_window)
        self.tree2["columns"] = ("Non-terminals", "Index")
        self.tree2.column("#0", width=0, stretch=tk.NO)
        self.tree2.column("Non-terminals", anchor=tk.W, width=200)
        self.tree2.column("Index", anchor=tk.W, width=200)
        self.tree2.heading("#0", text="", anchor=tk.W)
        self.tree2.heading("Non-terminals", text="Non-terminals", anchor=tk.W)
        self.tree2.heading("Index", text="Index", anchor=tk.W)
        self.tree2.grid(row=0, column=1, padx=10, pady=10, sticky="nsew")

        file_path = "./PROCESS_FILES/0.LexSynCheckList.txt"  # 替换为实际文件路径
        cnt = 0
        with open(file_path, "r", encoding="utf-8") as file:
            for line in file:
                tokens = line.split()
                if cnt < 85:
                    self.tree1.insert("", "end", values=(tokens[0], tokens[1]))
                else:
                    self.tree2.insert("", "end", values=(tokens[0], tokens[1]))
                cnt += 1

        # 文本框
        self.text_box = tk.Text(syntax_window, height=30, width=40)
        self.text_box.grid(row=1, column=0, columnspan=2, padx=10, pady=10, sticky="nsew")
        grammar_path = "./PROCESS_FILES/SyntaxGrammar_display.txt"
        with open(grammar_path, 'r') as file:
            content = file.read()
            self.text_box.delete("1.0", tk.END)  # 清空文本框
            self.text_box.insert(tk.END, content)  # 插入文件内容


        # Add three buttons at the bottom
        button_frame = tk.Frame(syntax_window)
        button_frame.grid(row=2, column=0, columnspan=2, pady=10)

        button1 = tk.Button(button_frame, text="LR1分析表", command=self.button1_action, width=15)
        button1.pack(side=tk.LEFT, padx=10)

        button2 = tk.Button(button_frame, text="规约过程", command=self.button2_action, width=15)
        button2.pack(side=tk.LEFT, padx=10)

        button3 = tk.Button(button_frame, text="语法树", command=self.button3_action, width=15)
        button3.pack(side=tk.LEFT, padx=10)

        # 设置行和列的权重，使其在窗口大小变化时可以调整
        for i in range(3):
            self.root.grid_rowconfigure(i, weight=1)
            self.root.grid_columnconfigure(i, weight=1)
    
    def button1_action(self):
        # Create horizontal and vertical Scrollbars
        new_w = tk.Toplevel(self.root)
        new_w.title("Action/GoTo 表")
        new_w.geometry("850x700")  # Set window size

        sb = tk.Scrollbar(new_w, orient=tk.HORIZONTAL)
        sb.pack(side="bottom", fill="x")

        yb = tk.Scrollbar(new_w, orient=tk.VERTICAL)
        yb.pack(side="right", fill="y")

        # Create a Frame to hold the Treeview
        frame = tk.Frame(new_w)
        frame.pack(expand=True, fill="both")

        # Create the Treeview
        self.tree6 = ttk.Treeview(frame, xscrollcommand=sb.set)
        self.tree6["columns"] = tuple(V_ALL)
        self.tree6.column("#0", width=0, stretch=tk.NO)
        self.tree6.heading("#0", text="", anchor=tk.W)
        for i in range(len(V_ALL)):
            self.tree6.column(V_ALL[i], anchor=tk.W, width=80)
            self.tree6.heading(V_ALL[i], text=V_ALL[i], anchor=tk.W)

        # 从文件中读取内容并添加到表格中
        file_path = "./PROCESS_FILES/5.LR1Table.txt"  # 替换为实际文件路径
        with open(file_path, "r", encoding="utf-8") as file:
            for line in file:
                tokens = line.split()
                if len(tokens) > 2 and tokens[0] != "2147483647":
                    
                    self.tree6.insert("", "end", values=tuple(tokens))

        self.tree6.pack(expand=True, fill="both")

        sb.config(command=self.tree6.xview)
        yb.config(command=self.tree6.yview)
        
    def button2_action(self):
        # Create horizontal and vertical Scrollbars
        new_w = tk.Toplevel(self.root)
        new_w.title("规约过程")
        new_w.geometry("850x700")  # Set window size

        sb = tk.Scrollbar(new_w, orient=tk.HORIZONTAL)
        sb.pack(side="bottom", fill="x")

        yb = tk.Scrollbar(new_w, orient=tk.VERTICAL)
        yb.pack(side="right", fill="y")

        # Create a Frame to hold the Treeview
        frame = tk.Frame(new_w)
        frame.pack(expand=True, fill="both")

        # Create the Treeview
        self.tree7 = ttk.Treeview(frame, xscrollcommand=sb.set)
        self.tree7["columns"] = tuple(["步骤", "状态栈", "符号栈", "输入串", "动作说明"])
        self.tree7.column("#0", width=0, stretch=tk.NO)
        self.tree7.heading("#0", text="", anchor=tk.W)

        self.tree7.column("步骤", anchor=tk.W, width=100)
        self.tree7.heading("步骤", text="步骤", anchor=tk.W)
        self.tree7.column("状态栈", anchor=tk.W, width=600)
        self.tree7.heading("状态栈", text="状态栈", anchor=tk.W)
        self.tree7.column("符号栈", anchor=tk.W, width=1000)
        self.tree7.heading("符号栈", text="符号栈", anchor=tk.W)
        self.tree7.column("输入串", anchor=tk.W, width=3800)
        self.tree7.heading("输入串", text="输入串", anchor=tk.W)
        self.tree7.column("动作说明", anchor=tk.W, width=500)
        self.tree7.heading("动作说明", text="动作说明", anchor=tk.W)

        # 从文件中读取内容并添加到表格中
        file_path = "./PROCESS_FILES/6.AnalyzeProcess.txt"  # 替换为实际文件路径
        with open(file_path, "r") as file:
            FL = False
            for line in file:   
                if FL:              
                    self.tree7.insert("", "end", values=tuple(line.split("    ")))
                FL = True

        self.tree7.pack(expand=True, fill="both")

        sb.config(command=self.tree7.xview)
        yb.config(command=self.tree7.yview)

    def button3_action(self):
        
        new_w = tk.Toplevel(self.root)
        new_w.title("语法树")
        new_w.geometry("1600x1200")  # Set window size

        img = Image.open("./PROCESS_FILES/SynTaxTree.png")
        img = img.resize((1500,1000))
        img = ImageTk.PhotoImage(img)

        label = Label(new_w, image=img)
        label.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")

        new_w.mainloop()

if __name__ == "__main__":
    root = tk.Tk()
    app = FileAnalyzerApp(root)
    root.mainloop()
