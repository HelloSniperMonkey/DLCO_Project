;bubble sort

;this is the array
ldc 23    ;23
stl 0   
ldc 87    ;87
stl 1
ldc 32    ;32
stl 2
ldc 1     ;1
stl 3
ldc 5     ;5
stl 4  
ldc 4     ;4
stl 5
ldc 3     ;3
stl 6
ldc 2     ;2
stl 7
ldc 133   ;133
stl 8
ldc 67    ;67
stl 9 
ldc 10   
ldc 0  
stnl 100  
outer_loop: 
    ldc 0
    ldnl 100
    adc -1            
    ldc 0  
    stnl 100  ;temp1
    ldc 0     ;exit condition
    ldnl 100
    brz exit ; when memory[0+100]==0 IT WILL EXIT
    ldc 0
    ldc -1   ; initializing inner loop
    br inner ; inner loop will be called
inner:
     adc 2
     ldc 0
     add 
     adc -1   
     a2sp
     ldc 0
     ldnl 100 
     sp2a   
     adc 1  
     sub     
     brlz outer_loop 
     sp2a  
     adc 1  
     ldc 0  
     add
     adc -1   
     ldc 200 ;temp2
     stnl 0
     sp2a
     adc 1
     ldc 0         
     add           
     adc -1
     ldnl 0 
     a2sp
     ldnl 0  
     sp2a   
     sub                     
     brlz swap
     ldc 0
     ldnl 200   
     adc 1
     ldc 0
     add
     adc -1
     br inner         
swap:  
     sub 
     ldc 0
     ldnl 200
     ldnl 1
     a2sp
     ldc 0
     ldnl 200
     stnl 1
     sp2a
     sp2a
     ldc 0
     ldnl 200 ;temp2
     stnl 0 
     adc 1
     ldc 0       
     add         
     adc -1     
     br inner   
exit:  HALT    
    
    
     

     
    




