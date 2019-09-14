#################################################################################
#            INTERFACE GRAPHIQUE POUR LE TESTEUR ELECTRIQUE                     #
#################################################################################



from serial import *
from tkinter import *
import time

# le nom du serialPort varie d'un PC a un autre, on remplace COM5 par le COM correspondant a notre cas
serialPort = "COM5" 
baudRate = 9600
ser = Serial(serialPort , baudRate, timeout=0, writeTimeout=0) 

# on cree la fenetre TKinter (root)
root = Tk()
root.wm_title("TesterApp")
root.wm_iconbitmap('img/icon.ico')
root.configure(background='#0C0355')
root.geometry("700x600")
root.resizable(width=False, height=False)

#scrollbar
scrollbar = Scrollbar(root)
scrollbar.pack(side=RIGHT, fill=Y)

#le champs text pour afficher les resultats du test
log = Text ( root, width=100, height=35, takefocus=0, fg= "#0C0355", bg= "#FFFFFF")
log.pack()

# mmm .. c'est juste pour faire la liaison entre scrollbar et le champ du text..
log.config(yscrollcommand=scrollbar.set)
scrollbar.config(command=log.yview)

# buffer.. genre zone memoire sert a stocker des donnees
serBuffer = "" #buffer vide initialement


# delFunc().. c'est l'evenemnet a lancer quand on clique sur le bouton 'clear'
def delFunc():
    ser.flushOutput()
    ser.flushInput()
    log.delete('1.0', END)
    ser.write(b'7')
    time.sleep(.1)
    

# readSerial().. c'est l'evenement a lancer qu'on clique sur le bouton 'Play' 
def readSerial():
    ser.flushOutput()
    while True:
        ser.write(b'l')

        c = ser.read().decode("ascii") 
        if len(c) == 0:
            break
        global serBuffer
        if c == '\r':
            c = '' 
        if c == '\n':
            serBuffer += "\n" 
            log.insert(END, serBuffer)
            serBuffer = "" 
        else:
            serBuffer += c 
        
        
# chemins vers les icons.
play_img = PhotoImage(file = "img/play_icon.png")
clear_img = PhotoImage(file = "img/clear_icon.png")

# bouton play pour lancer le test ---> on clique DEUX FOIS pour une premiere utilisation
Button(root,bg = '#FFFFFF',image=play_img, width=64, height=64,command=readSerial).pack(side=RIGHT,padx=10, pady=2)

# bouton clear pour supprimer le contenu du champ texte
## a titre d'info.. on doit tt d'abord brancher les cables avant de cliquer sur clear suivi de Play
Button(root, bg= '#FFFFFF', image=clear_img, width=64, height=64, command=delFunc).pack(side=LEFT, padx=10, pady=2)

root.mainloop()