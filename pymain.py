reg_8 = ("ah", "al", "bh", "bl", "ch", "cl", "dh", "dl")
reg_16 = ("ax", "bx", "cx", "dx", "si", "di", "bp", "sp",)
MOT = ("add","sub","mov","lea","lds","xchg")
DT = ("db","dw","dd")
loc = 0
offset = 0
end_flag = 0
flag = 1
class sym_table() :
    def __init__(self,var, offset, segment, var_type) :
        self.var = var
        self.offset = offset
        self.segment = segment
        self.var_type = var_type
    def __contains__(self,var) :
        return hasattr(self,var)
    def __contains__(self,var_type) :
        return hasattr(self,var_type)

sym = []


def search_sym_table(var, var_type, n) :      #function to check whether the symbol exists
    global loc
    global offset
    #global end_flag
    offset = loc
    if " " in var :
        var = var.replace(" ", "!")
        error(var,3) 
    if var not in sym :
        if var_type == "segment" :            #checking for segment in symbol table
                for x in sym :
                    if (var_type in x):
                        loc = 0
                    else :
                        error("",2)    
                offset = loc
                loc = offset
                #end_flag+=1
                segment = "\titself"
        elif var_type == "db" :
            var_type = "Byte"
            offset = loc
            loc = offset + 8*n
            segment = "\tdata"
        elif var_type == "dw" :
            var_type = "Word"
            offset = loc
            loc = offset + 16*n
            segment = "\tdata"
        elif var_type == "dd" :
            var_type = "Doubleword"
            offset = loc
            loc = offset +32*n
            segment = "\tdata"
        else :
            offset = loc
            loc = offset
            segment = "\tcode"
    else :
        error(var,0)
    sym.append(sym_table(var.lstrip(), offset, segment, var_type))

def print_sym_table() :                     #to print symbol table
    print("\t",'='*70)
    print("        |     Symbol\t| Offset |       Segment    \t|         Type\t\t|")
    print("\t",'='*70)
    for i in sym :
        print("\t| ",i.var,"\t| ", i.offset,"H\t |", i.segment,"\t\t| ", i.var_type,"\t\t|")
    print("\t",'='*70)
    # f = open("out.txt","w")
    # f.write()

def check_start() :
    if "start" in sym:
        return True
    else :
        return False

def error(x, y) :
    if y == 0:
        print(x, "already declared")
    elif y == 1 :                                  #error for missing START
        print("START label not defined")
    elif y == 2 :
        print("segment declared before ending previous segment")
    elif y == 3 :
        print(x, "syntax error in variable name")

def calc_inst_size(instruction) :
    print(instruction)

def assemble(x) :
    global flag
    x=x.lower()
    n = 0
    if ";" in x :                           #to check for comments in a line
        pos = x.find(";")
        if len(x[0:pos]) != 0 :
            x = x[0:pos]
            assemble(x)
    elif "segment" in x :                   #to check for segment in a line
        pos = x.find("segment")
        var = x[0:pos].strip()
        search_sym_table(var, "segment", n)
    elif (":" in x) & (("assume" not in x) or ("cs:" not in x) or ("ds:" not in x)) :       #to check for label
        pos = x.find(":")
        var = x[0:pos]
        search_sym_table(var, "label", n)
    else :
        if flag == 1:
            for dt in DT :                      #loop to check for data directive
                dt=" "+dt
                if (dt in x) :
                    pos = x.find(dt)
                    var = x[0:pos].strip()
                    var1 = x[pos+2:len(x)]
                    n = var1.count(',') + 1
                    search_sym_table(var, dt.lstrip(), n)
                #     flag = 1
                # else :
                #     flag = 0
        # else :
        #     for mt in MOT :
        #         if mt in x :
        #             if check_start() :
        #                 calc_inst_size()
        #             else :
        #                 error("", 1)
        #     flag = 1



def sourceline() :                          #to read source file line by line from an external file
    File = open("program.txt","r")
    f = File.readlines()
    for x in f :
        assemble(x)    

def main() :
    '''Objective:
    '''
    #Approach
    sourceline()
    print_sym_table()

if __name__ == "__main__" :
    main()
