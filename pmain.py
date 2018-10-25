REG = ("al","ah","ax","bl","bh","bx","cl","ch","cx","dl","dh","dx","cs","ds")
MOT = ("add","sub","mov","hlt")
DT = ("db","dw","dd")
loc = 0
flag = 0           # To check whether a segment is defined , 0 is for data segment 1 is for code

class sym_table() :
    def __init__(self,var, offset, segment, var_type) :
        self.var = var
        self.offset = offset
        self.segment = segment
        self.var_type = var_type
    def __contains__(self,var) :
        return hasattr(self,var)

sym = []

def search_sym_table(var, var_type) :
    global loc
    if var not in sym :
        if var_type == "db" :
            var_type = "Byte Variable"
            offset = loc + 8
        elif var_type == "dw" :
            var_type = "Word Variable"
            offset = loc + 16
        elif var_type == "dd" :
            var_type = "Doubleword Variable"
            offset = loc + 32    
        segment = "Data_seg"
    loc = offset
    sym.append(sym_table(var, offset, segment, var_type))

def print_sym_table() :
    print("\t-------------------------------------------------------------------------",)
    print("        |     Symbol\t| Offset |       Segment    \t|         Type\t\t|")
    print("\t-------------------------------------------------------------------------",)
    for i in sym :
        print("\t| ",i.var,"\t| ", i.offset,"H\t |     ", i.segment,"\t| ", i.var_type,"\t|")
    print("\t-------------------------------------------------------------------------",)
    # f = open("out.txt","w")
    # f.write()

def assemble(x) :
    x=x.lower()
    if 
    for dt in DT :
        dt=" "+dt
        if (dt in x) :
            pos = x.find(dt)
            var = x[0:pos]
            search_sym_table(var, dt.lstrip())
    
        

def sourceline() :
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