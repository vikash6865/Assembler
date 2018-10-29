reg_8 = ("ah", "al", "bh", "bl", "ch", "cl", "dh", "dl")    #8bit registers
reg_16 = ("ax", "bx", "cx", "dx", "si", "di", "bp", "sp",)  #16bit registers
MOT = ("add","sub","mov","lea","lds","xchg","adc")      #machine opcode table
DT = ("db","dw","dd")                                   #data directive table
loc = 0                                       #location counter to calulate offset of instruction
offset = 0                                    #variable to hold offset of symbols
end_flag = 0                                  #flag to check end of a segment
sym_exist = 0                                 #flag to check if symbol already exists or not
line = 1                                      #for debugging reference
seg_count = 0                                 #to store the count of number of segment
sym = []                                      # list to hold sym_table object


class sym_table() :                           # class to encapsulate the data of symblo table
    def __init__(self,var, offset, segment, var_type) :     #to initialize data-members
        self.var = var                        #variable for symbol name
        self.offset = offset                  #variable for offset of symbol
        self.segment = segment                #variable for name of segment in which symbol belongs
        self.var_type = var_type              #variavle for type of symbol


def check_directive_segname() :               #function to return segment name in which the symbol belongs
    for ob in sym :
        if ob.var_type != "segment" :
            continue
        else :
            var = ob.var
    return var


def insert_in_sym(var, var_type, n) :         #function to insert symbol in sym_table if not already exist in symbol table
    global loc
    global offset
    global end_flag
    global seg_count
    offset = loc
    if var_type == "segment" :            #checking for segment in symbol table
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
    sym.append(sym_table(var.lstrip(), offset, segment, var_type))  #making new entry in symbol tabel


def search_sym_table(var, var_type, n) :      #function to check whether the symbol exists
    global sym_exist
    if " " in var :                           # check for syntax error in variable declared
        error(line,3)
    for ob in sym :
        if ob.var == var :                    #checking whether variable already exists in symbol table
            sym_exist = 1
            break
    if not sym_exist :
        insert_in_sym(var, var_type, n)       #if symbol does not exist then make new entry in symbol table 
    else :
        error(var,0)                          #if entry exists already then error is flashed


def print_sym_table() :                     #to print symbol table
    print("\t",'='*72)
    print("        |     Symbol\t| Offset |       Segment    \t|         Type\t\t|")
    print("\t",'='*72)
    for i in sym :
        print("\t| ",i.var,"\t| ", i.offset,"H\t |", i.segment,"\t\t| ", i.var_type,"\t\t|")
    print("\t",'='*72)
    # f = open("out.txt","w")
    # f.write()


def check_start() :                         #to check whether start label is declared or not
    for x in sym :
        if x.var != "start" :
            continue
        else :
            return True
    return False


def error(x, y) :                           #function to flash error
    if y == 0:
        print(x, "symbol already declared")
    elif y == 1 :
        print("START label not defined")
    elif y == 2 :
        print("segment declared before ending previous segment")
    elif y == 3 :
        print("syntax error at line ",line, "in variable name")
    elif y == 4 :
        print("Ends declared before declaration of segment")
    quit()


def calc_inst_size(instruction) :
    if instruction == "add" :
        x = 1


def check_directive(x) :                    #program to check for data directive
    for dt in DT : 
        dt=" "+dt+" "
        if (dt not in x) :
            continue
        else :                              #if data directive found then insert in symbol table
            pos = x.find(dt)
            var = x[0:pos].strip()          #var holds name of variable
            var1 = x[pos+3:len(x)].strip()  #var1 holds data values 
            var2 = 0
            if "dup(?)" in var1 :           #check whether dup operator exists or not
                pos = var1.find("dup(?)")
                var2 = var1[0:pos].strip()
                n = int(var2)
            else :
                n = var1.count(',') + 1         #n holds number of data values to be stored in single data directive
            dt = dt.strip()
            search_sym_table(var, dt, n)    #var passed to search for finding if there is previously defined var
            return True
    return False                            #function returns if data directive not found


def assemble(x) :
    global end_flag
    x=x.lower()
    n = 0
    if len(x) == 1 :
        return
    elif ";" in x :                           #to check for comments in a line
        pos = x.find(";")
        if len(x[0:pos]) != 0 :
            x = x[0:pos]
            assemble(x)
    elif "segment" in x :                     #to check for segment in a line
        pos = x.find("segment")
        var = x[0:pos].strip()
        search_sym_table(var, "segment", n)
    elif (":" in x) & (("assume" not in x) or ("cs:" not in x) or ("ds:" not in x)) :       #to check for label
        pos = x.find(":")
        var = x[0:pos]
        search_sym_table(var, "label", n)
    elif "ends" in x :                        #to check for end of segment
        f = 0
        for ob in sym :
            if ob.var_type == "segment" :     #checking whether a segment is defined or not
                f = 1
                break
        if f == 1 :
            end_flag+=1
        else :
            error("",4)                       #flashses error if ends is defined but there is no segment in program
    else :
        if not check_directive(x) :
            for mt in MOT :
                    mt = " "+mt+" "
                    if mt in x :
                        if check_start() :               #to check wheteher start label is declared or not 
                            pos = x.find(mt)
                            mt = mt.strip()

                            calc_inst_size(mt)
                        else :
                            error("", 1)                 #flashes error if START label is missing


def sourceline() :                          #to read source file line by line from an external file
    File = open("program.txt","r")
    f = File.readlines()
    global line
    for x in f :
        # print("line",line)
        line+=1
        assemble(x)    


def main() :
    '''Objective:
    '''
    #Approach
    sourceline()
    print_sym_table()


if __name__ == "__main__" :
    main()