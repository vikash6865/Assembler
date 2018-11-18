#importing python packages
import re

#declaring global variables
reg_8 = ("ah", "al", "bh", "bl", "ch", "cl", "dh", "dl")             #8bit registers
reg_16 = ("ax", "bx", "cx", "dx", "si", "di", "bp", "sp", "ds")      #16bit registers
MOT = ("add","sub","mov","lea","inc","dec","neg","adc")              #machine opcode table
DT = ("db","dw","dd")                                                #data directive table
loc = 0                                                              #location counter to calulate offset of instruction
offset = 0                                                           #variable to hold offset of symbols
end_flag = 0                                                         #flag to check end of a segment
sym_exist = 0                                                        #flag to check if symbol already exists or not
line = 1                                                             #for debugging reference
seg_count = 0                                                        #to store the count of number of segment
sym = []                                                             # list to hold sym_table object


class sym_table() :                                                  # class to encapsulate the data of symblo table
    def __init__(self,var, offset, segment, var_type) :              #to initialize data-members
        self.var = var                                               #variable for symbol name
        self.offset = offset                                         #variable for offset of symbol
        self.segment = segment                                       #variable for name of segment in which symbol belongs
        self.var_type = var_type                                     #variavle for type of symbol


def check_directive_segname() :
    '''
        Objective : Function to return segment name in which the symbol belongs.
    '''
    #Approach: Loop through symbol table and match segment name , if segment found return segment name.
    for ob in sym :
        if ob.var_type != "segment" :
            continue
        else :
            var = ob.var
    return var


def insert_in_sym(var, var_type, n) :
    '''
        Objective: function to insert symbol in sym_table if symbol is not exists in symbol table.
            Input:
                var = name of symbol
                var_type = type of symbol variable
                n = number of values in variable
    '''
    #   Approach: by appending the new sym object in list
    global loc
    global offset
    global end_flag
    global seg_count
    offset = loc
    if var_type == "segment" :                                     #checking for segment in symbol table
        for x in sym :
            if ( var_type == x.var_type ) & (x.var != var) :       #if a segment is already defined clear location counter
                if end_flag >= seg_count :                         #checking whether previous segment declared has ends directive
                    loc = 0
                else :
                    error("",2)                                    #flashes error if ends is missing for previous segment
        offset = loc                                               #updating offset, loc, segment name, segment count 
        loc = offset
        segment = "\titself"
        seg_count+=1
    elif var_type == "db" :                                        #checking for byte variable
        var_type = "Byte"
        offset = loc
        loc = offset + 8*n
        segment = "\t"+check_directive_segname()                   #updating name of segment to which variable belongs
    elif var_type == "dw" :                                        #checking for word variable
        var_type = "Word"
        offset = loc
        loc = offset + 16*n
        segment = "\t"+check_directive_segname()
    elif var_type == "dd" :                                        #checking for double word variable
        var_type = "Doubleword"
        offset = loc
        loc = offset +32*n
        segment = "\t"+check_directive_segname()
    elif var_type == "label" :                                     #checking for label
        offset = loc
        loc = offset
        segment = "\t"+check_directive_segname()
    sym.append(sym_table(var.lstrip(), offset, segment, var_type)) #making new entry in symbol tabel


def search_sym_table(var, var_type, n) :
    '''
        Objective: function to check whether the symbol exists.
            Input:
                var = name of the symbol
                var_type = type of symbol
                n = number of values in variable
    '''
    #   Approach: loop through the symbol table and then comparing symbol variable name with the new variable
    global sym_exist
    global line
    if " " in var :                                                # check for syntax error in variable declared
        error(line,3)
    for ob in sym :
        if ob.var == var :                                         #checking whether variable already exists in symbol table
            sym_exist = 1
            break
    if not sym_exist :
        insert_in_sym(var, var_type, n)                            #if symbol does not exist then make new entry in symbol table 
    else :
        error(var,0)                                               #if entry exists already then error is flashed


def print_sym_table() :
    '''
        Objective: Function to print symbol table.
    '''
    #   Approach: using print statements
    print("\t"*4,"#### Symbol Table ####")
    print("\t",'='*72)
    print("        |     Symbol\t| Offset |       Segment    \t|         Type\t\t|")
    print("\t",'='*72)
    for i in sym :
        print("\t| ",i.var,"\t| ", i.offset,"H\t |", i.segment,"\t\t| ", i.var_type,"\t\t|")
    print("\t",'='*72)


def check_start() :
    '''
        Objective: to check whether start label is declared or not.
    '''
    #   Approach: by checking for start label in symbol table
    for x in sym :
        if x.var != "start" :
            continue
        else :
            return True
    return False


def error(x, y) :
    '''
        Objective: Function to flash errors
            Input:
                x = extra parameter
                y = error type
    '''
    #   Approach: comparing error type and then print corresponding error.
    global line
    print("\t"*4,"!!!! Error !!!!")
    if y == 0:
        print(x, "symbol already declared")
    elif y == 1 :
        print("START label not defined")
    elif y == 2 :
        print("segment declared before ending previous segment")
    elif y == 3 :
        print("syntax error at line ",line-1, "in variable name")
    elif y == 4 :
        print("Ends declared before declaration of segment")
    elif y == 5 :
        print(x," instruction missing 2nd operand")
    elif y == 6 :
        print(x," operands size mismatch at line ",line-1)
    elif y == 7 :
        print("both operands cant be same at line ",line-1)
    elif y == 8 :
        print("both operands cant be memory locations, error at line ",line-1)
    elif y == 9 :
        print("syntax error at line",line-1,"ptr used here has invalid syntax")
    elif y ==10 :
        print("variable used does not exist in symbol table at line",line-1)
    elif y == 11 :
        print("segment left without ends")
    quit()


def check_memory_var(op2) :
    '''
        Objective: Function to named memory location operand.
            Input:
                op2 = operand of the instruction
    '''
    #   Approach: by checking if variable exist in symbol table or not.
    if " " in op2:
        error("",3)
    else :
        for ob in sym :
            if op2 == ob.var :
                return True
        return False        


def check_immediate(op2) :
    '''
        Objective: Function to identify immediate operand.
            Input:
                op2 = 2nd operand of the instruction
    '''
    #   Approach: by checking for digits type whether binary or hex or octal or decimal  
    if " " in op2:
        error("",3)
    else :
        if ('h' or 'd' or 'o' or 'b') in op2 :
            return True
    return False


def validate_ptr(bef_ptr, aft_ptr) :
    '''
        Objective: Function to validate syntax of Ptr operand.
            Input:
                bef_ptr = left of ptr
                aft_ptr = right of ptr
    '''
    #   Approach: using regular expression
    if " " in bef_ptr :
        error("",3)
    elif re.match("[\[ax|bx|cx|dx|si|di\]]",aft_ptr) :
        pos = aft_ptr.find("[")
        var_name = aft_ptr[0:pos]
        if not check_memory_var(var_name) :
            error(var_name,10)
        else :
            for ob in sym :
                if var_name == ob.var :
                    var_type = ob.var_type
                    if ((bef_ptr == "byte" or bef_ptr == "word") and (var_type == "Doubleword")) :
                        return True
                    elif((bef_ptr == "byte") and (var_type == "word")) :
                        return True
    else :
        if '+' in aft_ptr :
            pos = aft_ptr.find('+')
            var_name = aft_ptr[0:pos]
        elif '-' in aft_ptr :
            pos = aft_ptr.find('-')
            var_name = aft_ptr[0:pos]
        else :
            var_name = aft_ptr
        if not check_memory_var(var_name) :
            error(var_name,10)
        else :
            for ob in sym :
                if var_name == ob.var:
                    var_type = ob.var_type
                    if ((bef_ptr == "byte" or bef_ptr == "word") and (var_type == "Doubleword")) :
                        return True
                    elif((bef_ptr == "byte") and (var_type == "word")) :
                        return True
    return False


def check_singop_mode(operands) :
    '''
        Objective: Function to calculate offset for single operand instruction.
            Input:
                operands = single operand
    '''
    #   Approach: comapare the operands with different type of register and variable and then based on that 
    #             calculate offset.
    global offset
    op = operands.strip()
    if " " in operands :
        error("",3)
    else :
        if (op in reg_8) :
            mode = "register_direct8"
            offset+=1
        elif (op in reg_16) :
            mode = "register_direct16"
            offset+=2
        elif (check_memory_var(op) or re.match("\[ax|bc|cx|dx|si|di\]",op)) :
            mode = "direct_addressing"
            offset+=2

def check_dualop_mode(operands) :
    '''
        Objective : Function to calculate offset based on the addressing mode.
            Inputs:
                operands = two operands
    '''
    #   Approach : check for different addressing modes by splitting the operands and then by identifying the operands type.
    global offset
    pos = operands.find(",")
    op1 = operands[0 : pos].strip()                                                 #op1 holds the first operand
    op2 = operands[pos+1 : len(operands)].strip()                                   #op2 holds the second operand
    if op1 == op2 :
        error("",7)
    elif (((op1 in reg_8 or op1 in reg_16) and " ptr " in op2) or 
    (" ptr " in op1 and (op2 in reg_8 or op2 in reg_16))) :                         #one operand is register and other is memory location with ptr
        if "ptr" in op1:
            op = str(op1)
        else :
            op = str(op2)
        pos = op.find("ptr")
        bef_ptr = str(op[0:pos]).strip()
        aft_ptr = str(op[pos+len("ptr"):len(op)]).strip()
        if validate_ptr(bef_ptr,aft_ptr) :                                          #validate_ptr() checks for syntax for ptr 
            mode = "direct_addressing"
            if ((op1 in reg_8 and bef_ptr == "byte") or
             (op2 in reg_8 and bef_ptr == "byte")) :
                offset+=2
            elif ((op1 in reg_16 and bef_ptr == "word") or
             (op2 in reg_16 and bef_ptr == "word")) :
                offset+=4
        else :
            error("",9)
    elif ((op1 in reg_8 or op1 in reg_16) and (check_immediate(op2))) :             #checking whether 2nd operand is immediate
        mode = "immediate_mode"
        if op1 in reg_8 :
            offset+=2
        else :
            offset+=4
    elif (op1 in reg_8 and op2 in reg_8) :                                          #checking for both operanda as 8 bit register
        mode = "register_direct8"
        offset+=2
    elif (op1 in reg_16 and op2 in reg_16):                                         #checking for both operanda as 16 bit register
        mode = "register_direct16"
        offset+=4
    elif (((op1 in reg_8 or op1 in reg_16) and
     re.match("\[ax|bx|cx|dx|si|di\]",op2)) or
     ((re.match("\[ax|bx|cx|dx|si|di\]",op1) and
      (op2 in reg_8 or op2 in reg_16)))) :                                          #checking for one operand as register and 2nd as memory location
        mode = "direct_addressing"
        if op1 in reg_8 or op2 in reg_8 :
            offset+=2
        elif op1 in reg_16 or op2 in reg_16 :
            error("",6)
    elif (((op1 in reg_8 or op1 in reg_16) and check_memory_var(op2)) or
     (check_memory_var(op1) and (op2 in reg_8 or op2 in reg_16))) :                 #checking for one operand as register and 2nd as variable
        mode = "direct_addressing"
        if op1 in reg_8 or op2 in reg_8 :
            offset+=2
        elif op1 in reg_16 or op2 in reg_16 :
            offset+=4
    elif ((op1 in reg_8 and op2 in reg_16) or (op1 in reg_16 and op2 in reg_8)) :   #checking for operand size mismatch
        error(op1+" , "+op2,6)
    elif check_memory_var(op1) and check_memory_var(op2) :                          #checking for both operands as memory location
        error("",8)
    # print("\t\tIP:",offset)


def calc_inst_size(instruction, operands) :
    '''
        Objective: Function to calculate offset of instruction.
            Input:
                instruction = instruction type
                operands = operands for that instruction
    '''
    #   Approach: check for different instruction type using if else statements. if instruction matches 
    #             then check for the types of operands. based on the types of operand and instruction calculate offset.
    global offset
    global loc
    if (instruction == "add" or instruction == "sub" or instruction == "adc") :     #check for additon, subtraction, add with carry instruction
        offset+=1                                                                   #for add, sub, adc , instruction size is 1
        if ',' not in operands :
            error("add",5)
        else :
            check_dualop_mode(operands)                                             #calculates instruction size for double operands instruction
            loc = offset
    elif (instruction == "mov") :
        offset+=2                                                                   #for mov, instruction size is 2
        if ',' not in operands :
            error("mov",5)
        else :
            check_dualop_mode(operands)
            loc = offset
    elif (instruction == "inc" or instruction == "dec" or instruction == "neg") :   #check for increment, decrement, negation instruction
        offset+=1
        check_singop_mode(operands)                                                 #calculates instruction size for single operand instruction
        loc = offset


def check_directive(x) :
    '''
        Objective: function to check for data directive
                Input :
                    x = assembly source code line
    '''
    #   Approach: loop through data diretive tuple and check whether any data directive exists in source line
    #             if data directive is found then identify its name and its type and check whether its exists
    #             in symbol table or not.
    for dt in DT : 
        dt=" "+dt+" "
        if (dt not in x) :
            continue
        else :                                                   #if data directive found then insert in symbol table
            pos = x.find(dt)
            var = x[0:pos].strip()                               #var holds name of variable
            var1 = x[pos+3:len(x)].strip()                       #var1 holds data values 
            var2 = 0
            if "dup(?)" in var1 :                                #check whether dup operator exists or not
                pos = var1.find("dup(?)")
                var2 = var1[0:pos].strip()
                n = int(var2)
            else :
                n = var1.count(',') + 1                          #n holds number of data values to be stored in single data directive
            dt = dt.strip()
            search_sym_table(var, dt, n)                         #var passed to search for finding if there is previously defined var
            return True
    return False                                                 #function returns false if data directive not found


def assemble(x) :
    '''
        Objective: This function parses each line to identify the type of instruction.
                   Input: 
                        x = assembly code source line
    '''
    #   Approach: check for special identifiers in source line to identify the type of instruction.
    global end_flag
    global seg_count
    x=x.lower()
    n = 0
    if len(x) == 1 :
        return
    elif ";" in x :                                              #to check for comments in a line
        pos = x.find(";")
        if len(x[0:pos]) != 0 :
            x = x[0:pos]
            assemble(x)
    elif "segment" in x :                                        #to check for segment in a line
        pos = x.find("segment")
        var = x[0:pos].strip()
        search_sym_table(var, "segment", n)
    elif (":" in x) & (("assume" not in x) or 
        ("cs:" not in x) or ("ds:" not in x)) :                  #to check for label
        pos = x.find(":")
        var = x[0:pos]
        search_sym_table(var, "label", n)
    elif "ends" in x :                                           #to check for end of segment
        f = 0
        for ob in sym :
            if ob.var_type == "segment" :                        #checking whether a segment is defined or not
                f = 1
                break
        if f == 1 :
            end_flag+=1
        else :
            error("",4)
    elif "end start" in x :                                      #checking end of start
        if end_flag != seg_count :
            error("",11)
    else :
        if not check_directive(x) :                              #checking whether line contains directive or a machine instruction
            for mt in MOT :
                    mt = " "+mt+" "
                    if mt in x :
                        if check_start() :                       #to check wheteher start label is declared or not 
                            x = x.strip()
                            var = x[len(mt)-1 : len(x)].strip()  #var holds the operands for the instruction
                            mt = mt.strip()                      #mt holds the instruction type
                            calc_inst_size(mt, var)
                        else :
                            error("", 1)                         #flashes error if START label is missing


def sourceline() :
    '''
        Objective: This function reads assembly file and then processes it line by line.
    '''
    #   Approach: create file object and then read it line by line and then pass each line to assemble
    #             function for further processing.
    File = open("program.txt","r")
    f = File.readlines()
    global line
    for x in f :
        print("line",line,"||  ",x)
        line+=1
        assemble(x)
    print("\n!!file parsed successfully!!")


def main() :
    '''
        Objective: This is the main driver function.
    '''
    #   Approach: Call Function read file line by line and then process it to generate symbol table and print it.
    sourceline()
    print_sym_table()


if __name__ == "__main__" :
    main()