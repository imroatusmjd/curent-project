import tkinter as tk
from tkinter import scrolledtext
import threading
import cv2
from PIL import Image, ImageTk


# Variabel global untuk status lapangan dan jalankan
status_lapangan = 0
jalankan = 0

# Fungsi untuk menangani klik tombol On
def turn_on():
    global show_output
    show_output = True

# Fungsi untuk menangani klik tombol Off
def turn_off():
    global show_output
    show_output = False
    output_label.config(image='')

# Fungsi untuk menangani klik tombol Lapangan A
def pilih_lapangan_a():
    global status_lapangan
    status_lapangan = 1
    print(f"Status Lapangan: {status_lapangan}")

# Fungsi untuk menangani klik tombol Lapangan B
def pilih_lapangan_b():
    global status_lapangan
    status_lapangan = 2
    print(f"Status Lapangan: {status_lapangan}")

# Fungsi untuk menangani klik tombol Play
def play_action():
    global jalankan
    jalankan = 1
    print(f"Jalankan: {jalankan}")

# Fungsi untuk menampilkan popup Lapangan A dan B
def show_popup(event):
    popup_menu.post(event.x_root, event.y_root)

# Fungsi untuk menangani klik tombol Menu 1
def menu1_action(event):
    show_popup(event)

# Fungsi untuk menangani klik tombol Menu 2
def menu2_action():
    print("Menu 2 clicked")

# Fungsi untuk menjalankan tflite.py
def run_tflite():
    cap = cv2.VideoCapture(0)
    
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return
    
    def update_frame():
        ret, frame = cap.read()
        if not ret:
            print("Error: Failed to capture image.")
            return

        if show_output:
            display_output(frame)
        
        output_label.after(10, update_frame)

    update_frame()

# Fungsi untuk menampilkan output
def display_output(frame):
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    img = Image.fromarray(frame)
    imgtk = ImageTk.PhotoImage(image=img)
    output_label.imgtk = imgtk
    output_label.config(image=imgtk)

# Membuat jendela utama
root = tk.Tk()
root.title("DEWAYANI UNESA")
root.geometry("800x480")
root.configure(bg="white")

# Warna dan font
bg_color = "#A52A2A" # Warna coklat tua
text_color = "white"
font = ("Arial", 12, "bold")

# Membuat dan menempatkan tombol Menu
menu1_button = tk.Button(root, text="Menu 1", bg=bg_color, fg=text_color, font=font)
menu1_button.place(x=50, y=50, width=100, height=40)
menu1_button.bind("<Button-1>", menu1_action)

menu2_button = tk.Button(root, text="Menu 2", command=menu2_action, width=10, bg=bg_color, fg=text_color, font=font)
menu2_button.place(x=170, y=50, width=100, height=40)

# Membuat popup menu untuk pilihan Lapangan
popup_menu = tk.Menu(root, tearoff=0)
popup_menu.add_command(label="Lapangan A", command=pilih_lapangan_a)
popup_menu.add_command(label="Lapangan B", command=pilih_lapangan_b)

# Membuat dan menempatkan tombol Nilai
nilai_x_button = tk.Button(root, text="Nilai X", bg=bg_color, fg=text_color, font=font)
nilai_x_button.place(x=50, y=400, width=100, height=40)

nilai_y_button = tk.Button(root, text="Nilai Y", bg=bg_color, fg=text_color, font=font)
nilai_y_button.place(x=170, y=400, width=100, height=40)

# Membuat dan menempatkan tombol On dan Off
on_button = tk.Button(root, text="ON", command=turn_on, bg=bg_color, fg=text_color, font=font)
on_button.place(x=290, y=400, width=100, height=40)

off_button = tk.Button(root, text="OFF", command=turn_off, bg=bg_color, fg=text_color, font=font)
off_button.place(x=410, y=400, width=100, height=40)

# Membuat dan menempatkan tombol Play
play_button = tk.Button(root, text="Play", command=play_action, bg="#A52A2A", fg=text_color, font=font)
play_button.place(x=50, y=350, width=100, height=40)

# Membuat dan menempatkan label status
output_label = tk.Label(root, bg=bg_color)
output_label.place(x=290, y=50, width=460, height=340)

# Menambahkan label nama universitas
uni_name_label = tk.Label(root, text="UNIVERSITAS NEGERI SURABAYA", font=font, bg="white", fg=bg_color, justify="center")
uni_name_label.place(x=50, y=450, width=700, height=30)

# Menjalankan tflite.py di thread terpisah
show_output = False
thread = threading.Thread(target=run_tflite)
thread.daemon = True
thread.start()

# Menjalankan aplikasi
root.mainloop()
