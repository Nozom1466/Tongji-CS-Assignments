import os
import string
from collections import defaultdict,deque
from queue import Queue
import csv
import numpy as np
import copy as cp
from pathlib import Path
import subprocess
import graphviz

folder = "./PROCESS_FILES/"
V={}                                                            # 符号及标号对照表
MAX_N = 1000                                                   
table = [[-1 for _ in range(MAX_N)]for _ in range(MAX_N)]       # 预测分析表
tb_s_r = [[-1 for _ in range(MAX_N)] for _ in range(MAX_N)]     # 移进项/规约项,-1/-2
numvt = 0                                                       # 终结符个数
num = 0                                                         # 终结符个数 + 非终结符个数
ItemSet = []                                                    # 项目集族
edge = [[0, 0, 0] for _ in range(MAX_N)]                        # 项目集族边
head = [0 for _ in range(MAX_N)]                                # 第i个项目集的头
nume = 0                                                        # 边数
word = []                                                       # 词法分析器分词结果

def check_valid_path(path):
    """检查文件路径是否有效，并返回以'/'分割的文件路径"""
    if '\\' in path:
        elements = str(Path(path)).split(sep='\\')
        final_path = Path('/'.join(elements))
    elif '/' in path:
        final_path = Path(path)
    else:
        if not Path(path).exists():
            raise Exception("Error: File path pattern is not correct.")
        else:
            final_path = Path(path)
    if not final_path.exists():
        raise Exception("Error: Your file path does not exist.")
    if final_path != '':
        return final_path
    else:
        return None

def dot2png(dot_file_path=None, img_path=None):
    """决策树可视化中.dot文件转化为.png图片的函数"""
    if not dot_file_path:
        raise Exception(".dot file is not given.")
    elif not dot_file_path.endswith('.dot'):
        raise Exception("file provided is not '.dot' type.")

    DOT_PATH = check_valid_path(dot_file_path)

    if not img_path:
        img_path = 'dt_png.png'
    elif not img_path.endswith('.png'):
        raise Exception("image file not end with '.png'.")

    IMG_PATH = img_path

    cmd_args = ['dot', '-Tpng', DOT_PATH, '-o', IMG_PATH]

    cmd_pro = subprocess.Popen(args=cmd_args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    retval = cmd_pro.stdout.read().decode('gbk')
    if retval == '':
        print("successfully create file " + IMG_PATH)
    else:
        print("The program encountered some error: ")
        print(retval)

class Item:
    def __init__(self, nump=0, ppos=0, forward=[]):
      self.nump=nump
      self.ppos=ppos
      self.forward = forward
  
    def __eq__(self, other):
        return self.nump == other.nump \
        and self.ppos==other.ppos \
        and set(self.forward)==set(other.forward)  # we do not care the order!

class Tree:
    def __init__(self, key, value, children=[]):
        self.key = key
        self.value = value
        self.children = []

#依据格式输入文法
#前一个参数输入，后三个参数输出
def inputGrammar(file_path, get_num, get_string, production):
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table
    n1, n2, n3 = 0, 0, 0    #分别记录终结符、非终结符、产生式的数目
    n = 0
    get_num['2147483647'] = n  # 注意 get_num的key是 str!!!!!!!!!!
    get_string[n] = 2147483647  # INT_MAX 代表#号
    n += 1
    with open(file_path, 'r') as f:
        n1, n2, n3 = map(int, f.readline().split())
        numvt = n1 + 1
        num = n1 + n2 + 1

        # terminals
        terminals = f.readline().split()
        for i in range(len(terminals)):
            get_num[terminals[i]] = n
            get_string[n] = terminals[i]
            n += 1;
        
        get_num['-1000'] = n 
        get_string[n] = -1000
        n += 1

        # non-terminals
        non_terminals = f.readline().split()
        for i in range(len(non_terminals)):
            get_num[non_terminals[i]] = n
            get_string[n] = non_terminals[i]
            n += 1;

        # grammars
        for i in range(n3):
            file_line_buf = list(map(int, f.readline().split()))
            grammar_tmp = file_line_buf[1:]
            production.append(grammar_tmp)
    
#合并左式相同的产生式
#前两个参数为输入，后一个参数为输出
def getProduction(production, get_num, get_produce, get_string):
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    for i in range(len(production)):
        temp = get_num[str(production[i][0])]  # 获得产生式左边的非终结符的编号
        get_produce[temp].append(i)

def dfsGetFirst(production, getNum, getProduce, first, nv, nump, getfirst):
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    temp = getNum[str(production[nump][1])]
    getfirst[nump] = True

    if temp <= numvt:
        first[nv].append(temp)
    else:
        for j in range(1, len(production[nump])):
            temp = getNum[str(production[nump][j])]
            for i in range(len(getProduce[temp])):
                if production[nump][0] == production[nump][1]:
                    continue
                dfsGetFirst(production, getNum, getProduce, first, temp, getProduce[temp][i], getfirst)
            
            if numvt not in first[temp]:  # numvt -----> \epsilon
                first[nv] = first[nv] + first[temp]
                break

            tmp_first_temp = cp.deepcopy(first[temp])
            tmp_first_temp.remove(numvt)
            first[nv] = first[nv] + tmp_first_temp
            if j == len(production[nump]) - 1:
                first[nv].append(numvt)

def getFirst(production, getNum, getProduce, first, getString):
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    getfirst = [False for _ in range(MAX_N)]  # 初始化
    for i in range(1, numvt + 1):  # 终结符first集合是它自己。 first集合通过终结符的编号来存储
        first[i].append(i)

    for i in range(len(production)):
        if (production[i][0] == production[i][1]):
            continue
        if (getfirst[i]):
            continue
        temp = getNum[str(production[i][0])]
        dfsGetFirst(production, getNum, getProduce, first, temp, i, getfirst)

    for i in range(len(first)):
        first[i] = list(set(first[i]))
    
def change_production(production):
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    for i in range(len(production)):
        if production[i][1] == -1000:
            production[i].remove(-1000)

def init():     #初始化函数
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    global nume
    for i in range(MAX_N):
        head[i] = -1

    for i in range(MAX_N):
        for j in range(MAX_N):
            tb_s_r[i][j] = table[i][j] = -1
    nume = 0

def addedge(from1,to,w):  #添加边
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table
    edge[nume][0] = to 
    edge[nume][1] = head[from1] 
    head[from1] = nume 
    edge[nume][2] = w 
    nume += 1

#项目、项目集运算
def itemInSet(a,b): 
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table
    for i in range(len(b)): 
        if a==b[i]: 
            return True 
    return False 
 
def itemSetMerge(a,b): 
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table
    for i in range(len(b)): 
        if not itemInSet(b[i],a): 
            a.append(b[i]) 
    return a 
 
def itemSetEqual(a, b):  # 两个项目集是否相等
   if len(a) != len(b):
       return False
   for i in range(len(a)):
       if not itemInSet(a[i], b):
           return False
   return True

def findItemSet(a,b): 
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    for i in range(len(b)): 
        if itemSetEqual(a, b[i]): 
            return i 
    return -1

#求解项目集闭包
def getItemClosure(t, get_num, get_produce, production, first): 
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table
    temp = [t] 
    q = Queue() 
    q.put(t) 
    while not q.empty(): 
        cur = q.get() 
        if cur.ppos == len(production[cur.nump]): 
            continue 
        tt = get_num[str(production[cur.nump][cur.ppos])] 
        if tt <= numvt: 
            continue 
        for i in range(len(get_produce[tt])): 
            c = Item(ppos=1, nump=get_produce[tt][i]) 

            if len(production[cur.nump]) - cur.ppos == 1: 
                c.forward = cur.forward + c.forward 
            else: 
                for j in range(1, len(production[cur.nump])): 
                    tttnum = get_num[str(production[cur.nump][cur.ppos + j])]
                    
                    if numvt not in first[tttnum]:
                        c.forward = first[tttnum] + c.forward
                        break
                    tmp_first_tttnum = cp.deepcopy(first[tttnum])
                    tmp_first_tttnum.remove(numvt)
                    c.forward = c.forward + tmp_first_tttnum 
                    if cur.ppos + j == len(production[cur.nump]) - 1: 
                        c.forward = cur.forward + c.forward 
                        break 
            if not itemInSet(c, temp): 
                q.put(c) 
                temp.append(c) 
    return temp

#获得项目集族
def getItemSet(get_num, get_produce, production, first): 
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table
    global q, t
    temp = [] 
    t = Item(nump=0, ppos=1, forward=[0]) 
    temp = getItemClosure(t, get_num, get_produce, production, first)
    q = Queue() 
    q.put(temp) 
    ItemSet.append(temp)  # S -> .BB, # 
 
    while not q.empty(): 
        cur = q.get()
        for i in range(1, num + 1):  # 所有符号 
            if i == numvt: 
                continue  # 空字符 
            temp = [] 
            for j in range(len(cur)): 
                if cur[j].ppos == len(production[cur[j].nump]):  # 是规约项目，无法再读入 
                    continue 
                tt = get_num[str(production[cur[j].nump][cur[j].ppos])] 
                if tt == i: 
                    tempt = Item(ppos=cur[j].ppos + 1 , nump=cur[j].nump, forward=cur[j].forward) 
                    temp = itemSetMerge(temp, getItemClosure(tempt, get_num, get_produce, production, first)) 
            if len(temp) == 0: 
                continue  # 该符号无法读入 
            num_cur = findItemSet(cur, ItemSet)  # 当前节点标号 
            tttnum = findItemSet(temp, ItemSet)  # 新目标标号 
            if tttnum == -1:  # 新的项目集 
                ItemSet.append(temp) 
                q.put(temp) 
                addedge(num_cur, len(ItemSet)-1, i)  # 添加边，权为读入的符号 
            else: 
                addedge(num_cur, tttnum, i) 

#打印项目集族
def printItemSet(production, getString):
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    for i in range(len(ItemSet)):
        print("项目集" + str(i) + ":")
        for j in range(len(ItemSet[i])):
            print("{" + ",".join(map(str, production[ItemSet[i][j].nump])) + "}", ItemSet[i][j].ppos, ItemSet[i][j].forward)
        print()
    for i in range(len(ItemSet)):
        j = head[i]
        while j != -1:
            print("  " + getString[edge[j][2]] + "\n" + str(i) + "--->" + str(edge[j][0]))
            j = edge[j][1]

#获得LR1分析表 table[i][j] = w:状态i --> j,读入符号W 
def getLR1Table(production): 
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    print(f"Size of ItemsSet: {len(ItemSet)}") 
    for i in range(len(ItemSet)):        #遍历图 
        j = head[i]
        while j != -1:                #遍历边 
            if table[i][edge[j][2]] != -1: 
                return False                   #多重入口，报错. 
            table[i][edge[j][2]] = edge[j][0] 
            tb_s_r[i][edge[j][2]] = -1          #移近项-1 
            j = edge[j][1]
    for i in range(len(ItemSet)):        #遍历所有项目 
        for j in range(len(ItemSet[i])): 
            if ItemSet[i][j].ppos == len(production[ItemSet[i][j].nump]):        #归约项 
                for k in range(len(ItemSet[i][j].forward)): 
                    if table[i][(ItemSet[i][j].forward)[k]] != -1: 
                        return False           #多重入口，报错. 
                    if (ItemSet[i][j].forward)[k] == 0 and ItemSet[i][j].nump == 0: 
                        table[i][(ItemSet[i][j].forward)[k]] = -3   #接受态 
                    else: 
                        table[i][(ItemSet[i][j].forward)[k]] = ItemSet[i][j].nump 
                        tb_s_r[i][(ItemSet[i][j].forward)[k]] = -2   #归约态 
    return True

#打印LR1分析表
def printLR1Table(getString, LR1_info_path):
    global numvt, num, nume, word, V, head, ItemSet, edge, tb_s_r, table

    LR1_info_file = open(LR1_info_path, 'w')

    with open("./PROCESS_FILES/5.LR1Table.txt", "w") as out:
        out.write(f"{len(ItemSet)} {num}\n")

        for j in range(num + 1):
            if j == numvt:
                continue
            out.write(f"    {getString[j]}")
        out.write('\n')
        for i in range(len(ItemSet)):
            for j in range(num + 1):
                if j == numvt:
                    continue
                msg = ""
                if table[i][j] == -3:
                    msg = "acc " # accept
                elif table[i][j] == -1:
                    msg = "* "  # epsilon
                elif tb_s_r[i][j] == -1:
                    msg = f"s{table[i][j]} " # shift
                elif tb_s_r[i][j] == -2: 
                    msg = f"r{table[i][j]} " # reduce

                out.write(msg)
                if msg != "* ":
                    LR1_info_file.write(f"{i} {j} {msg}\n")
            out.write("\n")

    LR1_info_file.close()

################################################################################################
#打印当前状态
def printCurState(count, state, wd, i, get_string, out):
    out.write(str(count) + "    ")
    flag = True
    temp = deque()
    while len(state) > 0:
        temp.append(state[-1])
        state.pop()
    while len(temp) > 0:
        out.write(str(temp[-1]))
        out.write(',')
        flag = False
        temp.pop()
    if flag:
        out.write("NUL")

    flag = True
    out.write("    ")
    while len(wd) > 0:
        temp.append(wd[-1])
        wd.pop()
    while len(temp) > 0:
        i_ = get_string[temp[-1]]
        if i_ == 2147483647:
            out.write("#,")
        else:
            out.write(V[str(i_)])
            out.write(',')
        temp.pop()
        flag = False
    if flag:
        out.write("NUL")

    flag = True
    out.write("    ")
    for j in range(i, len(word) - 1):
        out.write(V[str(word[j])])
        flag = False
    if flag:
        out.write("NUL")
    out.write("    ") 

#打印树函数
def printTree(root, depth):
    for i in range(depth):
        print(" ", end="")
    print(root.value)
    for i in range(len(root.children)):
        printTree(root.children[i], depth + 2)

#绘制树函数
def paintTree1(out, root):
    if len(root.children) > 0:
        out.write(f"{root.key}[label=\"{V[str(root.value)]}\"];\n")
    else:
        out.write(f"{root.key}[shape=polygon,sides=4,peripheries=2,color=lightblue,style=filled,label=\"{V[str(root.value)]}\"];\n")
    for i in range(len(root.children)):
        paintTree1(out, root.children[i])

def paintTree2(out, root):
    for i in range(len(root.children)):
        out.write(f"{root.key}->{root.children[i].key};\n")
    for i in range(len(root.children)):
        paintTree2(out, root.children[i])

#生成树函数
def getTree(regular_set, printTreeTerminal):
    """
    Args:
        regular_set: deque([])
    """
    key = 0  # 用来唯一标识某个树节点

    root=Tree(0,0)
    Var = deque()  # 变量存储栈
    cur = regular_set[-1]
    regular_set.pop()

    if len(cur) == 1:  # 空产生式
        cur.append('-1000')
    root = Tree(key, cur[0])
    key += 1

    for i in range(1, len(cur)):
        t = Tree(key, cur[i])
        key += 1
        root.children.append(t)
        Var.append(t)

    while len(regular_set) > 0:  # LR1分析是最左规约，对应最右推导
        cur = regular_set[-1]
        regular_set.pop()
        if len(cur) == 1:  # 空产生式
            cur.append(-1000)
        p = Var[-1]
        while p.value >= 0 or p.value == -1000:
            Var.pop()
            if len(Var) == 0:
                break
            p = Var[-1]
        if len(Var) == 0:
            break
        if p.value != cur[0]:
            print("出错！")
            return

        Var.pop()  # 弹出p
        for i in range(1, len(cur)):
            t = Tree(key, cur[i])
            key += 1
            p.children.append(t)
            Var.append(t)

    if printTreeTerminal:
        printTree(root, 0)  # 这个函数可以打印文字版的tree，但是标号类型的
    with open("./PROCESS_FILES/SynTaxTree.dot", "w") as out:
        out.write("digraph G {\n")
        out.write("rankdir = TB;\n")
        paintTree1(out, root)
        paintTree2(out, root)
        out.write("}\n")

    dot2png("./PROCESS_FILES/SynTaxTree.dot", "./PROCESS_FILES/SynTaxTree.png")

#总控程序
def totalControl(getString, getNum, production, printTree):
    with open("./PROCESS_FILES/6.AnalyzeProcess.txt", "w") as out:
        regularset = deque()  # 规约顺序
        print("步骤", "状态栈", "符号栈", "输入串", "动作说明", sep=" ", file=out)
        state = deque()  # 状态栈和符号栈
        state.append(0)

        wd = deque()  # '#'
        wd.append(0)
        count = 0
        i = 0
        while True:
            cur = state[-1]
            if table[cur][getNum[str(word[i])]] == -1:  # 空白，报错误
                return 0
            if table[cur][getNum[str(word[i])]] == -3:  # 接受态
                regularset.append(production[0])  # 添加规约顺序
                state_ = cp.deepcopy(state)
                wd_ = cp.deepcopy(wd)
                printCurState(count, state_, wd_, i, getString,out)
                count += 1
                out.write("acc!\n")
                getTree(regularset, printTree)
                return 1
            if tb_s_r[cur][getNum[str(word[i])]] == -1:  # 移进项
                state_ = cp.deepcopy(state)
                wd_ = cp.deepcopy(wd)
                printCurState(count, state_, wd_, i, getString,out)
                count+=1
                print("action[", cur, ",", V[str(getNum[str(word[i])])], "]=", table[cur][getNum[str(word[i])]], ",", "状态", table[cur][getNum[str(word[i])]], "入栈", sep=" ", file=out)
                wd.append(getNum[str(word[i])])
                state.append(table[cur][getNum[str(word[i])]])
                i += 1
            elif tb_s_r[cur][getNum[str(word[i])]] == -2:  # 归约
                state_ = cp.deepcopy(state)
                wd_ = cp.deepcopy(wd)
                printCurState(count, state_, wd_, i, getString, out)
                count += 1
                numpro = table[cur][getNum[str(word[i])]]
                len_ = len(production[numpro]) - 1
                for j in range(len_):
                    state.pop()
                    wd.pop()
                wd.append(getNum[str(production[numpro][0])])
                cur1 = state[-1]  # -----------------------------------
                print("用", V[str(production[numpro][0])], "->", sep=" ", end="", file=out)
                for j in range(1, len_ + 1):
                    print(V[str(production[numpro][j])], end=",", file=out)
                regularset.append(production[numpro])  # 添加规约顺序
                print("进行归约,", "goto[", cur1, ",", V[str(getNum[str(word[i])])], "]=", table[cur1][getNum[str(production[numpro][0])]], ",", "入栈", sep=" ", file=out)
                state.append(table[cur1][getNum[str(production[numpro][0])]])

        return 1

# 判断终止函数
def is_end(s):
    if s in ["int", "void", "if", "while", "else", "return"]:
        return True
    if s in ["+", "-", "*", "/", "=", "==", ">", "<", "!=", ">=", "<="]:
        return True
    if s in [";", ",", "(", ")", "{", "}", "ID", "NUM"]:
        return True
    if s in ["a", "b", "c", "d"]:
        return True
    return False

#备份函数
def LRTableBackup1(filename, row, col):
    with open(filename, 'w') as out:
        out.write(str(row))
        out.write(' ')
        out.write(str(col))
        out.write(str('\n'))
        for i in range(row):
            for j in range(col):
                out.write(str(table[i][j]))
                out.write(' ')
            out.write('\n')

def LRTableBackup2(filename, row, col):
    with open(filename, 'w', newline='') as out:
        writer = csv.writer(out)
        writer.writerow([row, col])
        for i in range(row):
            for j in range(col):
                writer.writerow([tb_s_r[i][j]])

def LRTableReturn1(filename):
    row, col = 0, 0
    with open(filename, 'r') as rin:
        row, col = map(int, rin.readline().split())
        table = [[0] * col for _ in range(row)]
        for i in range(row):
            for j in range(col):
                table[i][j] = int(rin.readline())

def LRTableBackup2(filename, row, col):
   with open(filename, 'w') as out:
       out.write(f"{row} {col}\n")
       for i in range(row):
           for j in range(col):
               out.write(f"{tb_s_r[i][j]} ")
           out.write("\n")

def LRProductionBackup(filename, pro):
    with open(filename, 'w') as out:
        out.write(str(len(pro)) + '\n')
        for it in pro:
            out.write(str(len(it)) + ' ')
            for itt in it:
                out.write(str(itt) + ' ')
            out.write('\n')

def LRProductionReturn(filename, pro):
    with open(filename) as rin:
        row = int(rin.readline())
        pro.clear()
        for i in range(row):
            tmp = []
            num = int(rin.readline())
            for j in range(num):
                k = int(rin.readline())
                tmp.append(k)
            pro.append(tmp)

def Syn_main():
    global folder, V, MAX_N, table, tb_s_r, numvt, num, ItemSet, edge, head, nume, word
    word = []
    table = [[-1 for _ in range(MAX_N)]for _ in range(MAX_N)]       # 预测分析表
    tb_s_r = [[-1 for _ in range(MAX_N)] for _ in range(MAX_N)]     # 移进项/规约项,-1/-2
    numvt = 0                                                       # 终结符个数
    num = 0                                                         # 终结符个数 + 非终结符个数
    ItemSet = []                                                    # 项目集族
    edge = [[0, 0, 0] for _ in range(MAX_N)]                        # 项目集族边
    head = [0 for _ in range(MAX_N)]                                # 第i个项目集的头
    nume = 0                                                        # 边数
    word = []                                                       # 词法分析器分词结果

    filePath = ""
    getNum = {}  # int, int
    getString = {}  # int, int
    production = []  # 2 dims
    getProduce = [[] for _ in range(MAX_N)]
    first = [[] for _ in range(MAX_N)]
    printTreeTerminal = False
    init()
    inputGrammar(folder + "4.SyntaxGrammar.txt", getNum, getString, production)

    with open(folder + "0.LexSynCheckList.txt", "r") as in1:
        for line in in1:
            symbol, index = line.split()
            V[index] = symbol
    getProduction(production, getNum, getProduce, getString)
    getFirst(production, getNum, getProduce, first, getString)  # 去重已经包含
    change_production(production)
    getItemSet(getNum, getProduce, production, first)
    printItemSet(production, getString)

    # LR1 table:  x y content
    LR1_info_path = folder + "LR1_info_tuple.txt"

    if not getLR1Table(production):
        print("This Grammar is not a LR(1) Grammar !")
        return -2
    printLR1Table(getString, LR1_info_path)  # This function has been modified, and the output is now saved in a file
    LRTableBackup1(folder + "backup_Table.txt", len(ItemSet) + 1, num + 2)
    LRTableBackup2(folder + "backup_tb_s_r.txt", len(ItemSet) + 1, num + 2)
    LRProductionBackup(folder + "backup_production.txt", production)

    with open(folder + "2.LexResultList.txt", "r") as inpu:
        for line in inpu:
            if int(line) < 0:
                break
            word.append(int(line[:-1]))
        word.append(2147483647)

    if not totalControl(getString, getNum, production, printTreeTerminal):
        print("error!")
        return -1
    else:
        return 0

def syn_exe():
    return Syn_main()

if __name__ == "__main__":
    Syn_main()
