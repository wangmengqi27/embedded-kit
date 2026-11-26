# embedded-kit

车载 CAN 节点（数据采集 + 故障诊断）的 PC 端代码库。
目标：**硬件无关的 app 层全部在这里写完、测完**，寒假接上 STM32 后只补 bsp 层。

## 现在的进度

| 阶段 | 内容 | 状态 |
|---|---|---|
| 阶段 0 | 环境：gcc / make / git / VS Code | ✅ 2026-11-26 |
| 阶段 1 | 基础模块：ringbuf、crc16、can_frame | 🟡 ringbuf 完成（20 项测试全过） |
| 阶段 2 | 协议层：can_proto + 周期调度 | ⬜ |
| 阶段 3 | 双节点 PC 仿真 + 丢帧统计 | ⬜ |
| 阶段 4 | 诊断：DTC + 超时检测 | ⬜ |
| 阶段 5 | 上硬件（寒假） | ⬜ |

## 目录

```
embedded-kit/
├── learn/          学习用小程序，一个文件一个练习（不参与正式模块）
├── src/app/        纯逻辑、硬件无关的模块（本项目主体）
├── src/bsp/        硬件相关（寒假填）
├── tests/          每个模块的 PC 端测试
├── tools/          Python 上位机等工具
└── README.md
```

## 环境（已装好，2026-11-26）

| 工具 | 版本 | 位置 |
|---|---|---|
| gcc | 16.2.0 (MSYS2 UCRT64) | `C:\msys64\ucrt64\bin`（已加入用户 PATH） |
| make | GNU Make 4.4.1 | `C:\msys64\usr\bin` |
| git | 2.56.1 | `D:\git\Git` |
| VS Code | — | 已装 C/C++ 扩展 |

> 新开一个终端（或 VS Code 里的终端）才能看到 PATH 的变化。

## 第一个程序：day01_hello

```powershell
cd D:\embedded-kit

# 编译：把 .c 变成 .exe
gcc -std=c11 -Wall -Wextra -O2 -g learn/day01_hello.c -o learn/day01_hello.exe

# 运行
.\learn\day01_hello.exe
```

预期输出：

```
frame #37
  temp = 35.20 C
  volt = 12.100 V
sizeof(SensorFrame_t) = 6 byte(s)
```

## 第一个模块：ringbuf（环形缓冲区）

```powershell
cd D:\embedded-kit

# 编译：被测模块 + 测试程序 一起编译
gcc -std=c11 -Wall -Wextra -O2 -g tests/test_ringbuf.c src/app/ringbuf.c -o tests/test_ringbuf.exe

# 运行测试
.\tests\test_ringbuf.exe
```

预期最后一行：`ALL TESTS PASSED`（20 项检查）。

> 现在每个模块只有一两个文件，一条 gcc 命令够用；等 crc16、can_frame 都写完，
> 再加 Makefile 统一管理（`make test` 一条命令跑全部）。

## 下一步

写 `src/app/crc16.c/.h`（CRC-16/CCITT 查表法），配 `tests/test_crc16.c`。
