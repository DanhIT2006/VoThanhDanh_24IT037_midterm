# Báo cáo Giữa kỳ: Implement ls(1)
**Sinh viên:** Võ Thành Danh
**MSSV:** 24IT037

## 1. Cấu trúc chương trình
Dự án được thiết kế theo mô hình modular chia thành 2 thư mục chính:
- `include/`: Chứa các file header (.h) khai báo cấu trúc và nguyên mẫu hàm.
- `src/`: Chứa các file source (.c) xử lý logic (options, traverse, display, sort).

## 2. Các cờ (flags) đã triển khai
Chương trình hỗ trợ các cờ theo chuẩn man page:
- `-a`, `-A`: Hiển thị file ẩn / bỏ qua `.` và `..`
- `-l`, `-n`: Hiển thị định dạng dài (thông tin phân quyền, chủ sở hữu, dung lượng, thời gian).
- `-h`, `-k`: Hiển thị kích thước file dễ đọc (Human-readable) hoặc theo Kilobytes.
- `-R`: Đọc thư mục đệ quy.
- `-r`, `-S`, `-t`: Đảo ngược sắp xếp, sắp xếp theo kích thước, sắp xếp theo thời gian sửa đổi.
(Và các cờ khác: -c, -d, -F, -f, -i, -q, -s, -u, -w)

## 3. Hướng dẫn biên dịch và sử dụng
- **Biên dịch:** Chạy lệnh `make` tại thư mục gốc.
- **Sử dụng:** `./ls [flags] [file/directory]` (Ví dụ: `./ls -la /src`)
- **Dọn dẹp:** Chạy lệnh `make clean`.