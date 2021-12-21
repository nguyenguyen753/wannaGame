## Brief Write Up:
File thứ 1: Xài thuật toán hash cơ bản để kiểm tra password, từ đó sẽ sinh ra process mới.

File thứ 2: RC4 + sha256 để mã hoá shellcode + xài software breakpoint để đặt anti debugger. Đồng thời xài api hashing để tránh static analyze.

Ý tưởng chung của bài là inject shellcode vào process notepad.exe, từ đó thực hiện reverse shell.