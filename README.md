## Petunjuk Penggunaan Program

Memanggil perintah 'make paralel' untuk program paralel dan 'make serial' untuk program serial. 

## Pembagian Tugas


| Nama | NIM | Pembagian Tugas |
| ------ | ------ | ------ |
| Lydia Astrella Wiguna | 13517019 | membuat fungsi-fungsi general, konfigurasi paralel dari serial, debugging |
| Karina Iswara | 13517031 | membuat algoritma dijkstra serial, konfigurasi paralel dari serial, debugging |

## Deskripsi Solusi Paralel

Solusi paralel didapatkan dari mengubah algoritma dijkstra pada serial. Solusi paralel didapatkan berdasarkan pemeriksaan jarak terdekat ke seluruh node yang dilakukan dari seluruh sumber node. Algoritma untuk mendapatkan jarak terdekat dari satu node sumber dilakukan oleh beberapa proses paralel. Sehingga pada akhirnya, nilai jarak-jarak terdekat ke seluruh node dari node sumber yang dilakukan setiap proses berbeda, disatukan dalam satu thread dan matrix jarak yang terbentuk diprint.

Pada program dijkstra paralel, pertama dilakukan inisialisai MPI dengan MPI_Init, pendeklarasian variable rank setiap thread, dan banyak proses. Lalu sesuai dengan banyak proses dan banyak thread, maka setiap thread diassign tugas menghitung jarak-jarak terdekat ke semua titik dari sumber node yang sebanyak banyak proses / banyak thread. Karena kami menggunakan asumsi jumlah proses adalah kelipatan jumlah thread maka hasil pembagian pasti berupa bilangan bulat. Setelah setiap thread selesai mencari jarak terdekat dari sumber-sumber node yang ditugaskan pada thread tersebut, dilakukan MPI_Gather untuk mengumpulkan array-array hasil operasi setiap thread ke thread 0. Lalu thread 0 meng-output hasil nilai dijkstra ke file txt.

## Analisis Solusi

Pada solusi paralel yang kami lakukan, waktu eksekusi yang dibutuhkan seharusnya menjadi lebih cepat karena pencarian jarak terdekat dari setiap sumber node dilakukan pada thread-thread yang berbeda secara bersamaan jika dibandingkan dengan program serial yang mengeksekusi jarak terdekat dari semua sumber node.

Seharusnya terdapat kemungkinan masih ada solusi yang memberikan kinerja lebih baik. Sebagai contohnya membuat paralel pada pencarian jarak terdekat dari satu titik (node) ke titik lainnya. Yakni mengimplementasikan algoritma paralel langsung dalam fungsi dijkstra yang terdapat dalam algoritma kami.

## Jumlah thread yang digunakan 

Thread yang digunakan sebanyak 5 karena asumsi yang kami gunakan adalah jumlah node sumber (N) harus merupakan kelipatan jumlah thread. Karena jumlah core maksimum adalah 6 dan yang memenuhi syarat kelipatan serta nilainya paling besar, maka nilai 5 dipilih sebagai jumlah thread yang digunakan.

## Hasil Pengukuran kinerja tiap kasus uji

### N = 100
Serial :


Paralel :


### N = 500
Serial :


Paralel :


### N = 1000
Serial :


Paralel :

### N = 3000
Serial :


Paralel :