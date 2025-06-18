# 🎵 Music Player - State Design Pattern in C

Dự án này mô phỏng một **Music Player** sử dụng **State Design Pattern** để quản lý hành vi khi người dùng nhấn nút `Play`, `Pause`, và `Stop`. Mỗi trạng thái của máy nghe nhạc (Playing, Pausing, Stopping) sẽ có cách xử lý sự kiện khác nhau, minh họa cho cách tổ chức mã sạch, dễ mở rộng và dễ bảo trì.

## 📁 Cấu trúc thư mục


├── inc
│   ├── musicplayer.h
│   ├── playerstate.h
│   └── state
│       ├── pausingstate.h
│       ├── playingstate.h
│       └── stoppingstate.h
├── main.c
├── Makefile
└── src
    ├── musicplayer.c
    └── state
        ├── pausingstate.c
        ├── playingstate.c
        └── stoppingstate.c

## 🧠 Mục tiêu

- Mô phỏng việc sử dụng State Pattern trong ngôn ngữ C (không có OOP).
- Tách biệt logic theo từng trạng thái để dễ bảo trì và mở rộng.
- Áp dụng con trỏ hàm và struct để mô phỏng giao diện và hành vi động.

## ⚙️ Cách chạy chương trình

1. **Biên dịch dự án:**

```bash
make
Chạy chương trình:
./musicplayer


▶️ Mô phỏng hành vi

Chương trình thực hiện chuỗi hành vi sau:

    Bắt đầu ở trạng thái Stopping.

    Nhấn Pause (không thực hiện được).

    Nhấn Play → chuyển sang trạng thái Playing.

    Nhấn Stop → chuyển về Stopping.

    Tiếp tục các hành vi để kiểm tra chuyển trạng thái.

Kết quả đầu ra (ví dụ):

Pressing pause button.
Can't Change to Pause State.
Pressing Play button.
Press playing button.
Music player in Playing mode.
Pressing Stop button.
Press Stopping button.
Music player in Stopping mode.
...
🏗️ Design Pattern sử dụng
✅ State Pattern

    PlayerState đóng vai trò là interface định nghĩa các hành vi: pressPlay, pressPause, pressStop.

    Các trạng thái cụ thể (PlayingState, PausingState, StoppingState) kế thừa từ interface này bằng cách cài đặt con trỏ hàm.

    MusicPlayer giữ trạng thái hiện tại và ủy quyền hành vi cho trạng thái đó.

💡 Ưu điểm

    Mã nguồn dễ đọc, dễ mở rộng.

    Dễ dàng thêm trạng thái mới như FastForwardState, RewindingState mà không sửa mã cũ.

    Phù hợp để mô phỏng các máy trạng thái trong embedded, game, hệ thống điều khiển,...



