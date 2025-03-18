import os
import subprocess
import shutil
from collections import deque

strStack = []
varStack = []
tempNum = 0
labelNum = 0

class LR0item:
    def __init__(self, item):
        self.item = item

    def getItem(self):
        return self.item

    def getLeft(self):
        return self.item[0]

    def getRight(self):
        return self.item[1]

    def __eq__(self, item):
        return self.getItem() == item.getItem()

    def __lt__(self, item):
        return self.getItem() < item.getItem()


class LR1item:
    def __init__(self, item):
        self.item = item

    def getItem(self):
        return self.item

    def getLeft(self):
        return self.item[0]

    def getRight(self):
        return self.item[1]

    def __eq__(self, item):
        return self.getItem() == item.getItem()

    def __lt__(self, item):
        return self.getItem() < item.getItem()

class Token:
    def __init__(self):
        self.name = ""
        self.type = None
        self.line = 0

    def setName(self, name):
        self.name = name

    def getName(self):
        return self.name

    def setType(self, type):
        self.type = type

    def getType(self):
        return self.type

    def setLine(self, line):
        self.line = line

    def getLine(self):
        return self.line

    def getTypeOutput(self):
        if self.type == TokenType.KEYWORD:
            return "KEYWORD"
        if self.type == TokenType.ID:
            return "ID"
        if self.type == TokenType.INT:
            return "INT"
        if self.type == TokenType.FLOAT:
            return "FLOAT"
        if self.type == TokenType.CHAR:
            return "CHAR"
        if self.type == TokenType.OPERATOR:
            return "OPERATOR"
        if self.type == TokenType.DELIMITER:
            return "DELIMITER"
        if self.type == TokenType.OTHER:
            return "OTHER"
        return None



def sem_getClosure(closure):
    temp = set()

    while True:
        if temp == closure:
            break

        temp = closure.copy()

        for lr1 in closure:
            lr0 = lr1.getLeft()
            lookahead = lr1.getRight()

            pointPos = lr0.getRight()
            right = grammar[lr0.getLeft()].getRight()

            if pointPos != len(right) and right[pointPos] in variableSet:
                B = right[pointPos]

                beta = right[pointPos + 1:] + [lookahead]

                first = getFirst(beta)

                for productionID, production in enumerate(grammar):
                    if production.getLeft() == B:
                        for terminal in first:
                            if production.getRight() == ["@"]:
                                newItem = LR1item(LR0item(productionID, 1), terminal)
                            else:
                                newItem = LR1item(LR0item(productionID, 0), terminal)
                            if newItem not in closure:
                                temp.add(newItem)

        closure = temp.copy()

    return closure


def buildFirstMap():
    firstMap = {}

    # basic
    for symbol in terminalSet:
        firstMap[symbol] = getFirst(symbol)
    for symbol in variableSet:
        firstMap[symbol] = getFirst(symbol)

    # merge
    while True:
        changed = False
        for symbol in variableSet:
            temp = firstMap[symbol].copy()
            for production in grammar:
                if production.getLeft() == symbol:
                    allEmpty = True
                    right = production.getRight()
                    for one in right:
                        oneSet = firstMap[one].copy()
                        if "@" in oneSet:
                            temp.update(oneSet)
                        else:
                            temp.update(oneSet)
                            allEmpty = False
                            break
                    if allEmpty:
                        temp.add("@")
            if temp != firstMap[symbol]:
                firstMap[symbol] = temp
                changed = True
        if not changed:
            break




def sem_analyse(tokens):
    tempNum = 0
    labelNum = 0
    sem_build()

    # 输出文件重定向
    fout = open("SysStack.txt", "w")
    if not fout:
        print("output open failed")
        exit(-1)

    print()
    print("Symbol stack information:")

    # 在此输出到文件
    fout.write("Symbol stack information:\n")

    # system("pause")

    st = deque()
    st.append((0, "$"))
    iter = iter(tokens)
    # 在这里输出程序运行中插入token的信息
    print("Inserted element:")
    token = next(iter)
    print(token.getName(), token.getTypeOutput())

    # 在此输出到文件
    fout.write("Inserted element:\n")
    fout.write(token.getName() + " " + token.getTypeOutput() + "\n")

    while True:
        I = st[-1][0]
        if token.getType() == TokenType.ID or token.getType() == TokenType.CHAR \
                or token.getType() == TokenType.INT or token.getType() == TokenType.FLOAT:
            type = TokenDict[token.getType()]
        else:
            type = token.getName()

        if type in action[I]:
            act = action[I][type]
            if act[0] == "S":
                if token.getType() == TokenType.ID or token.getType() == TokenType.CHAR \
                        or token.getType() == TokenType.INT or token.getType() == TokenType.FLOAT:
                    varStack.append(token.getName())
                    # print("[pushVAR]", token.getName(), "   [at line", token.getLine(), "]")
                st.append((act[1], type))

                # 在这里输出程序运行中栈的信息
                print("Stack Information:")
                for item in reversed(st):
                    print(item[0], item[1])
                print()

                # 在此输出到文件
                fout.write("Stack Information:\n")
                for item in reversed(st):
                    fout.write(str(item[0]) + " " + item[1] + "\n")
                fout.write("\n")

                # system("pause")

                token = next(iter)
                # 在这里输出程序运行中插入token的信息
                print("Inserted element:")
                print(token.getName(), token.getTypeOutput())

                # 在此输出到文件
                fout.write("Inserted element:\n")
                fout.write(token.getName() + " " + token.getTypeOutput() + "\n")
            elif act[0] == "r":
                id = act[1]
                right = grammar[id].getRight()
                for i in range(len(right)):
                    if right[i] != "@":
                        st.pop()
                newI = st[-1][0]
                st.append((go[newI][grammar[id].getLeft()], grammar[id].getLeft()))

                # translate
                # #ifdef test
                # print("use production[{}]: {}->{}".format(id, grammar[id].getLeft(), " ".join(right)))
                # #endif // test
                if not translate(id, token.getName()):
                    print("ERROR! at line", token.getLine())
                    return token.getLine()
                elif act[0] == "acc":
                    # 关闭SysStack.txt文件
                    fout.close()

                    fout = open("Quaternion_information.txt", "w")
                    if not fout:
                        print("output open failed")
                        exit(-1)

                    print("Accept!")
                    # 输出到文件
                    fout.write("Accept!\n")

                    # 在这里输出最后的四元式
                    print("Quaternion information:")
                    print(strStack[-1])
                    # 输出到文件
                    fout.write("Quaternion information:\n")
                    fout.write(strStack[-1] + "\n")
                    fout.close()


def sem():
    current_dir = os.path.dirname(os.path.abspath(__file__))
    exe_path = os.path.join(current_dir, "D_NEW_FOLDER", "Process.txt")
    directory, filename = os.path.split(exe_path)
    new_exe_path = os.path.join(directory, os.path.splitext(filename)[0] + ".exe")
    os.rename(exe_path, new_exe_path)
    print(exe_path)
    process = subprocess.Popen(new_exe_path)
    process.wait()
    os.rename(new_exe_path, exe_path)

    try:
        if os.path.exists(os.path.join(current_dir, "Quaternion_information.txt" )):
            shutil.move(
                os.path.join(current_dir, "Quaternion_information.txt" ),
                os.path.join(current_dir, 'D_NEW_FOLDER', "7.Quaternion_information.txt")
            )
        else:
            print("Quaternion Failed.")

        if os.path.exists(os.path.join(current_dir, "SysStack.txt")):
            shutil.move(
                os.path.join(current_dir, "SysStack.txt" ),
                os.path.join(current_dir, 'D_NEW_FOLDER', "8.SysStack.txt")
            )
        else:
            print("SysStack Failed.")

        os.remove(os.path.join(current_dir, "test.txt"))
        shutil.copy2(os.path.join(current_dir, "FirstSet.txt"), os.path.join(current_dir, "D_NEW_FOLDER", "9.FirstSet.txt"))
        os.remove(os.path.join(current_dir, "FirstSet.txt"))
        os.remove(os.path.join(current_dir, "Lexical_Results.txt"))
        os.remove(os.path.join(current_dir, "test_preprocessing.txt"))
    except:
        assert "Semantic Analysis Failed."
    pass


def sem_build():
    if len(closuremap) != 0:
        return

    buildFirstMap()

    closure0 = getClosure(LR1item(LR0item(0, 0), "$"))

    q = deque()
    q.append(closure0)

    closurelist.append(closure0)
    closuremap[closure0] = len(closuremap)

    while q:
        lr1set = q.popleft()
        transfer.append([])

        for variable in variableSet:
            newset = set()
            for lr1 in lr1set:
                lr0 = lr1.getLeft()
                id = lr0.getLeft()
                pointpos = lr0.getRight()
                if len(grammar[id].getRight()) > pointpos and grammar[id].getRight()[pointpos] == variable:
                    newset.add(LR1item(LR0item(id, pointpos + 1), lr1.getRight()))
            if newset:
                sem_getClosure(newset)
                if newset not in closuremap:
                    closurelist.append(newset)
                    closuremap[newset] = len(closuremap)
                    q.append(newset)
                transfer[-1].append((variable, closuremap[newset]))

        for terminal in terminalSet:
            newset = set()
            for lr1 in lr1set:
                lr0 = lr1.getLeft()
                id = lr0.getLeft()
                pointpos = lr0.getRight()
                if len(grammar[id].getRight()) != pointpos and grammar[id].getRight()[pointpos] == terminal:
                    newset.add(LR1item(LR0item(id, pointpos + 1), lr1.getRight()))
            if newset:
                sem_getClosure(newset)
                if newset not in closuremap:
                    closurelist.append(newset)
                    closuremap[newset] = len(closuremap)
                    q.append(newset)
                transfer[-1].append((terminal, closuremap[newset]))

    for i in range(len(closurelist)):
        action.append({})
        go.append({})

    for lr1 in closurelist[i]:
        point = lr1.getLeft().getRight()
        right = grammar[lr1.getLeft().getLeft()].getRight()
        if point < len(right) and right[point] in terminalSet:
            for go_pair in transfer[i]:
                if go_pair[0] == right[point]:
                    if right[point] in action[i] and action[i][right[point]] != ("S", go_pair[1]):
                        # error 1
                        print("error 1 at action[{}][{}]=S{}".format(i, right[point], go_pair[1]))
                        print("  old={}".format(action[i][right[point]]))
                    action[i][right[point]] = ("S", go_pair[1])

    for tf in transfer[i]:
        if tf[0] in variableSet:
            if tf[0] in go[i]:
                # error
                print("error at go[{}][{}]".format(i, tf[0]))
            go[i][tf[0]] = tf[1]

    for lr1 in closurelist[i]:
        lookahead = lr1.getRight()
        lr0 = lr1.getLeft()
        id = lr0.getLeft()
        point = lr0.getRight()
        right = grammar[id].getRight()

        if point == len(right):
            if lookahead in action[i] and action[i][lookahead] != ("r", id):
                # error 2
                print("error 2 at action[{}][{}]=r{} old={}".format(i, lookahead, id, action[i][lookahead]))
            # deal with if else / if
            if lookahead not in action[i] or action[i][lookahead] == ("r", id):
                action[i][lookahead] = ("r", id)

            if lookahead == "$" and id == 0:
                action[i][lookahead] = ("acc", 0)

def sem_translate(id, name):
    if id == 3:
        varStackPop()
        varStackPop()
        # test
        print("[case 3] [var pop]")
        print("[case 3] [var pop]")

    elif id == 9 or id == 10:
        str_val = strStackPop()
        os = StringIO()
        os.write("\n" + str_val)
        strStack.append(os.getvalue())

    elif id == 13:
        varStackPop()
        # test
        print("[case 13] [var pop]")

    elif id == 15:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        strStack.append(os.getvalue())
        rvar = varStackPop()
        lvar = varStackPop()
        varStack.append(lvar)
        # test
        print("[case 15] [str pop]")
        print("[case 15] [str pop]")
        print("[case 15] [str push]", os.getvalue())
    elif id == 24:
        r = varStackPop()
        l = varStackPop()
        os = StringIO()
        os.write(strStackPop())
        os.write(cmd("=", r, "", l) + "\n")
        strStack.append(os.getvalue())
        varStack.append(l)
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 27:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("or", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 29:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("and", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())
    elif id == 31:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("==", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 32:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("!=", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 34:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("<", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 35:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd(">", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 36:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("<=", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())
    elif id == 37:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd(">=", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 39:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("+", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 40:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("-", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 42:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("*", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id == 43:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        os.write(lstr + rstr)
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(cmd("/", lvar, rvar, itoTemp(tempNum)) + "\n")
        strStack.append(os.getvalue())
        varStack.append(itoTemp(tempNum))
        tempNum += 1
        # test
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str pop]")
        print("[case", id, "] [str push]", os.getvalue())

    elif id in [44, 45, 46, 47]:
        strStack.append("")
        # test
        print("[case", id, "] [str push] []")

    elif id == 49:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        rvar = varStackPop()
        lvar = varStackPop()

        os.write(cmd(itoLabel(labelNum), "", "", "") + "\n")
        os.write(lstr)
        os.write(cmd("J!=", lvar, "0", itoLabel(labelNum + 1)) + "\n")
        os.write(cmd("J", "", "", itoLabel(labelNum + 2)) + "\n")
        os.write(cmd(itoLabel(labelNum + 1), "", "", "") + "\n")
        os.write(rstr)
        os.write(cmd("J", "", "", itoLabel(labelNum)) + "\n")
    elif id == 37:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(lstr)
        os.write(cmd("J!=", lvar, "0", itoLabel(labelNum)) + "\n")
        os.write(cmd("J", "", "", itoLabel(labelNum + 1)) + "\n")
        os.write(cmd(itoLabel(labelNum), "", "", "") + "\n")
        os.write(rstr)
        os.write(cmd(itoLabel(labelNum + 1), "", "", "") + "\n")
        labelNum += 2
        strStack.append(os.getvalue())
        varStack.append(lvar)

    elif id == 52:
        rstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        rvar = varStackPop()
        lvar = varStackPop()
        os.write(lstr)
        os.write(cmd("J!=", lvar, "0", itoLabel(labelNum)) + "\n")
        os.write(cmd("J", "", "", itoLabel(labelNum + 1)) + "\n")
        os.write(cmd(itoLabel(labelNum), "", "", "") + "\n")
        os.write(rstr)
        os.write(cmd(itoLabel(labelNum + 1), "", "", "") + "\n")
        labelNum += 2
        strStack.append(os.getvalue())
        varStack.append(lvar)

    elif id == 53:
        rstr = strStackPop()
        midstr = strStackPop()
        lstr = strStackPop()
        os = StringIO()
        rvar = varStackPop()
        midvar = varStackPop()
        lvar = varStackPop()
        os.write(lstr)
        os.write(cmd("J!=", lvar, "0", itoLabel(labelNum)) + "\n")
        os.write(cmd("J", "", "", itoLabel(labelNum + 1)) + "\n")
        os.write(cmd(itoLabel(labelNum), "", "", "") + "\n")
        os.write(midstr)
        os.write(cmd("J", "", "", itoLabel(labelNum + 2)) + "\n")
        os.write(cmd(itoLabel(labelNum + 1), "", "", "") + "\n")
        os.write(rstr)
        os.write(cmd(itoLabel(labelNum + 2), "", "", "") + "\n")
        labelNum += 3
        strStack.append(os.getvalue())
        varStack.append(lvar)

    elif id in [44, 45, 46, 47]:
        strStack.append("")

    else:
        pass

    # test
    print("[/translate]", id, "  varSize=[", len(varStack), "]")
    print("    strSize=[", len(strStack), "]")
    print()

    return 1


