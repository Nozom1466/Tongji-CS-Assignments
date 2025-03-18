from enum import Enum

class VT(Enum):
    # 0 ~ 9
    VTNull = 0
    VTInteger = 1
    VTString = 2
    VTBoolean = 3
    VTFloating = 4
    VTCharacter = 5
    VTComment = 6
    VTMacro = 7
    VTWhiteSpace = 8
    VTEndLine = 9
    # 界符 括号 10 ~ 15
    VTLeftBrace = 10
    VTRightBrace = 11
    VTLeftSquareBracket = 12
    VTRightSquareBracket = 13
    VTLeftBracket = 14
    VTRightBracket = 15
    # 运算符 16 ~ 50
    VTPlus = 16
    VTMinus = 17
    VTMultiply = 18
    VTDivide = 19
    VTModulus = 20
    VTIncrement = 21
    VTDecrement = 22
    VTEqual = 23
    VTNotEqual = 24
    VTGreater = 25
    VTLess = 26
    VTGE = 27
    VTLE = 28
    VTLogicalAnd = 29
    VTLogicalOr = 30
    VTLogicalNot = 31
    VTBinaryAnd = 32
    VTBinaryOr = 33
    VTBinaryNot = 34
    VTXor = 35
    VTLeftShift = 36
    VTRightShift = 37
    VTAssignment = 38
    VTAddAssignment = 39
    VTSubtractAssignment = 40
    VTMultiplyAssignment = 41
    VTDivideAssignment = 42
    VTModulusAssignment = 43
    VTLeftShiftAssignment = 44
    VTRightShiftAssignment = 45
    VTBinaryAndAssignment = 46
    VTBinaryOrAssignment = 47
    VTBinaryXorAssignment = 48
    VTComma = 49
    VTSemicolon = 50
    # 关键字 51 ~ 82
    VTAuto = 51
    VTBreak = 52
    VTCase = 53
    VTChar = 54
    VTConst = 55
    VTContinue = 56
    VTDefault = 57
    VTDo = 58
    VTDouble = 59
    VTElse = 60
    VTEnum = 61
    VTExtern = 62
    VTFloat = 63
    VTFor = 64
    VTGoto = 65
    VTIf = 66
    VTInt = 67
    VTLong = 68
    VTRegister = 69
    VTReturn = 70
    VTShort = 71
    VTSigned = 72
    VTSizeof = 73
    VTStatic = 74
    VTStruct = 75
    VTSwitch = 76
    VTTypedef = 77
    VTUnion = 78
    VTUnsigned = 79
    VTVoid = 80
    VTVolatile = 81
    VTWhile = 82
    # words 标识符 82
    VTIdentifier = 83
    VTWord = 84

VT_keys = list(VT.__members__.keys())

VT_MIN = int(VT.VTNull.value)
VT_MAX = int(VT.VTWord.value)

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

def VT_to_string_(component):
    return VT_names[int(component.value) - int(VT.VTNull.value)]

def VT_to_type_(component):
    return VT_keys[int(component.value) - int(VT.VTNull.value)][2:]

def VT_to_string(i):
    if i < 0 or (i > VT_MAX - VT_MIN):
        return ""
    else:
        return VT_names[i]

def VTStr_to_int(p):
    for i in range(len(VT_names)):
        if p == VT_names[i]:
            return i + VT_MIN
    print("无法将" + p + "转化为既存终结符")
    return -1

def to_string(component):
    return VT_names[component]

def is_keyword(component):
    return 51 <= component < 83

def is_operator(component):
    return 16 <= component < 51

def is_bracket(component):
    return 10 <= component < 16

def is_value(component):
    return 1 <= component < 6

def is_identifier(identifier):
    return len(identifier) > 0 and identifier[0].isalpha() and all(c.isalnum() or c == '_' for c in identifier)

def ParseInteger(string):
    state = '0'  # 初始状态
    i = 0
    n = len(string)
    while state != '3' and state != '4' and i < n:
        ch = string[i]
        if state == '0':
            if '1' <= ch <= '9':
                state = '1'  # 1-9，转状态 1
            elif ch == '0':
                state = '2'  # 0，转状态 2
            else:
                state = '3'  # 其他，转状态 3
        elif state == '1':
            if '0' <= ch <= '9':
                state = '1'  # 0-9，转状态 1
            elif ch == ' ':
                state = '4'  # 结束，转 4
            else:
                state = '3'  # 其他，转状态 3
        elif state == '2':
            if ch == ' ':
                state = '4'  # 结束，转 4
            else:
                state = '3'  # 其他，转状态 3
        i += 1
    return state == '4'

def ParseFloating(string):
    state = '0'  # 初始状态
    i = 0
    n = len(string)
    while state != '9' and state != '8' and i < n:
        ch = string[i]
        if state == '0':
            if '1' <= ch <= '9':
                state = '1'  # 1-9，转状态 1
            elif ch == '0':
                state = '2'  # 0，转状态 2
            else:
                state = '8'  # 其他，转状态 8
        elif state == '1':
            if '0' <= ch <= '9':
                state = '1'  # 0-9，转状态 1
            elif ch == 'e' or ch == 'E':
                state = '5'  # e ，转状态 5
            elif ch == '.':
                state = '3'  # . ，转状态 3
            elif ch == ' ':
                state = '9'  # 结束，转 9
            else:
                state = '8'  # 其他，转状态 8
        elif state == '2':
            if ch == '.':
                state = '3'  # . ，转状态 3
            else:
                state = '8'  # 其他，转状态 8
        elif state == '3':
            if '0' <= ch <= '9':
                state = '4'  # 0-9，转状态 4
            else:
                state = '8'  # 其他，转状态 8
        elif state == '4':
            if '0' <= ch <= '9':
                state = '4'  # 0-9，转状态 4
            elif ch == 'e' or ch == 'E':
                state = '5'  # e ，转状态 5
            elif ch == ' ':
                state = '9'  # 结束，转 9
            else:
                state = '8'  # 其他，转状态 8
        elif state == '5':
            if '0' <= ch <= '9':
                state = '7'  # 0-9，转状态 7
            elif ch == '+' or ch == '-':
                state = '6'  # +/-，转状态 6
            else:
                state = '8'  # 其他，转状态 8
        elif state == '6':
            if '0' <= ch <= '9':
                state = '7'  # 0-9，转状态 7
            else:
                state = '8'  # 其他，转状态 8
        elif state == '7':
            if '0' <= ch <= '9':
                state = '7'  # 0-9，转状态 7
            elif ch == ' ':
                state = '9'  # 结束，转 9
            else:
                state = '8'  # 其他，转状态 8
        i += 1
    return state == '9'

def ParseIdentifier(string):
    state = '0'  # 初始状态
    i = 0
    n = len(string)
    while state != '3' and state != '2' and i < n:
        ch = string[i]
        i += 1
        if state == '0':
            if 'a' <= ch <= 'z' or 'A' <= ch <= 'Z' or ch == '_':
                state = '1'  # 转状态 1
            elif ch == ' ':
                state = '3'  # 转状态 3
            else:
                state = '2'
        elif state == '1':
            if 'a' <= ch <= 'z' or 'A' <= ch <= 'Z' or '0' <= ch <= '9' or ch == '_':
                state = '1'  # 转状态 1
            elif ch == ' ':
                state = '3'  # 转状态 3
            else:
                state = '2'
    return state == '3'

class LexSegment():
    def __init__(self, li=-1, ri=-1, ctype=VT.VTNull):
        self.li = li
        self.ri = ri
        self.ctype = ctype

class Res:
    def __init__(self, val=VT.VTNull, begin=-1, end=-1):
        self.val = val
        self.begin = begin
        self.end = end

class Trie:
    def __init__(self):
        self.isLeaf = False
        self.next = [None for _ in range(128)]
        self.type = None

    def insert(self, word, t):
        node = self
        for c in word:
            if node.next[ord(c) - ord('\0')] is None:
                node.next[ord(c) - ord('\0')] = Trie()
            node = node.next[ord(c) - ord('\0')]
        node.isLeaf = True
        node.type = t

    def match(self, word):
        node = self
        parent = self
        res = []
        start = 0
        ctl = 0
        len_word = len(word)

        while ctl < len_word - 1:
            node = self
            for i in range(start, len_word):
                c = word[i]
                # print(f"i: {i}, c: {c}")
                parent = node
                node = node.next[ord(c) - ord('\0')]

                if node is None:
                    if parent.isLeaf:
                        if (parent.type == VT.VTPlus or parent.type == VT.VTMinus) and (
                                i >= 2 and (word[i - 2] == 'e' or word[i - 2] == 'E')):
                            start = start + 1
                            ctl = i
                            break
                        res.append(Res(parent.type, start, i))
                        start = i
                    else:
                        start = start + 1

                    ctl = i
                    break
        return res

class LexAnalyzer:
    def __init__(self):
        self.original_code = None
        self.code_length = -1
        self.inner_result = []
        self.component_type = []
        self.trie = Trie()

        for i in range(10, 85):
            component = VT(i)
            sign = to_string(i)
            self.trie.insert(sign, component)

    def pre_analyze(self):
        is_single_line_comment = False  # 单行注释
        is_multiline_comment = False  # 多行注释
        is_string = False  # 字符串
        is_char = False  # 字符
        is_macro = False  # 宏指令
        start_index = 0
        for i in range(len(self.original_code)):
            cch = self.original_code[i]
            if is_macro:  # 宏指令
                if not cch or cch == '\n':
                    self.set_component(start_index, i, VT.VTMacro)
                    is_macro = False
            elif is_single_line_comment:  # 单行注释
                if not cch or cch == '\n':
                    self.set_component(start_index, i, VT.VTComment)
                    is_single_line_comment = False
            elif is_multiline_comment:  # 多行注释
                if cch == '/' and self.original_code[i - 1] == '*':
                    self.set_component(start_index, i + 1, VT.VTComment)
                    is_multiline_comment = False
            elif is_string:  # 字符串处理
                if cch == '\"' and self.original_code[i - 1] != '\\':
                    self.set_component(start_index, i + 1, VT.VTString)
                    is_string = False
            elif is_char:  # 字符处理
                if cch == '\'' and self.original_code[i - 1] != '\\':
                    self.set_component(start_index, i + 1, VT.VTCharacter)
                    is_char = False
            elif self.original_code[i:i + 2] == '/*' and i != 0:
                is_multiline_comment = True
                start_index = i
            elif self.original_code[i:i + 2] == '*/' and i != 0:
                is_multiline_comment = False
            elif self.original_code[i:i + 2] == '//' and i != 0:
                is_single_line_comment = True
                start_index = i - 1
            elif self.original_code[i] == '#':
                is_macro = True
                start_index = i
            if not cch:
                return

    def analyze_spliters(self):
        start_index = 0
        is_spliter = False
        tmp_code = [chr(c) for c in self.original_code]
        for i in range(len(tmp_code)):
            if self.component_type[i].value and not is_spliter:
                continue
            cch = tmp_code[i]
            if cch == ' ' or cch == '\t':
                if not is_spliter:
                    is_spliter = True
                    start_index = i
            else:
                if is_spliter:
                    self.set_component(start_index, i, VT.VTWhiteSpace)
                    is_spliter = False
            if cch == '\n':
                self.set_component(i, i + 1, VT.VTEndLine)
            elif not cch:
                break

    def analyze_keywords_and_operators(self):
        tmp_code = [chr(c) if c > 37 else ' ' for c in self.original_code]
        res = self.trie.match(tmp_code)
        for seg in res:
            if is_operator(seg.val.value):
                self.set_component(seg.begin, seg.end, seg.val)
        for seg in res:
            if is_bracket(seg.val.value):
                self.set_component(seg.begin, seg.end, seg.val)
        for seg in res:
            if is_keyword(seg.val.value):
                if (seg.begin == 0 or self.component_type[seg.begin - 1]) and (
                        seg.end == len(tmp_code) or self.component_type[seg.end]):
                    self.set_component(seg.begin, seg.end, seg.val)

    def analyze_words(self):
        start_index = -1
        tmp_code = "".join([chr(c) if c > 37 else ' ' for c in self.original_code])
        for i in range(len(tmp_code)):
            if start_index < 0 and (tmp_code[i] and not self.component_type[i].value):
                start_index = i
            elif start_index >= 0 and (not tmp_code[i] or self.component_type[i].value):
                code = tmp_code[start_index:i] + ' '
                if code == "true " or code == "false ":
                    self.set_component(start_index, i, VT.VTBoolean)
                elif ParseInteger(code):
                    self.set_component(start_index, i, VT.VTInteger)
                elif ParseFloating(code):
                    self.set_component(start_index, i, VT.VTFloating)
                elif ParseIdentifier(code):
                    self.set_component(start_index, i, VT.VTIdentifier)
                else:
                    self.set_component(start_index, i, VT.VTWord)
                start_index = -1

    def set_component(self, start_index, end_index, component):
        self.inner_result.append(LexSegment(start_index, end_index, component))
        for i in range(start_index, end_index):
            self.component_type[i] = component

    def Analyze(self, code):
        self.__init__()
        self.original_code = bytearray(code, 'utf-8')
        self.code_length = len(self.original_code)
        self.component_type = [VT.VTNull for _ in range(self.code_length + 1)]
        self.pre_analyze()
        self.analyze_spliters()
        self.analyze_keywords_and_operators()
        self.analyze_words()

    def Get_Result(self):
        return self.inner_result
