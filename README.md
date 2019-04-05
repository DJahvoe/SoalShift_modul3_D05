# SoalShift_modul3_D05

## Soal 1
- Problem : Menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan.

- Tools : <br/>
  - thread
  - pthread_create
  - pthread_join
  - struct
  
- Approach : <br/>
Untuk menyelesaikan permasalahan tersebut kita menggunakan thread agar penghitungan faktorial berjalan secara pararel. Dimulai dengan mendapatkan inputan dari user kemudian hasil inputan diurutkan, setiap kita menghitung faktorial dari tiap elemen array yang telah diurutkan pada thread, satu data dihitung oleh satu thread.

## Soal 4
- Problem : Menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. list proses disimpan dalam file  SimpanProses1.txt & SimpanProses2.txt  di direktori /home/Document/FolderProses1, tiap file dikompres format nama file KompresProses*.zip dengan file source dihapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file kompres.

- Tools : <br/>
  - thread
  - pthread_create
  - pthread_join
  - struct
  - sprintf()
  - zip -qmj
  - system()
  
- Approach : <br/>
Untuk menyelesaikan permasalahan tersebut kita menggunakan thread. Dengan ketentuan satu proses dilakukan oleh satu thread agar proses berjalan secara bersama-sama. Dalam melakukan command sistem kita menggunakan fungsi sprintf() yang fungsinya sama dengan stringcopy tetapi bisa mengenali format printf di c. Pertama-tama kita membuat direktori dengan 'system()' 'mkdir'. Selanjutnya untuk proses pengambilan list kita menggunakan 'ps aux --no-heading' agar heading seperti USER PID %CPU dsb tidak diambil. Hasil ps aux diambil dengan 'head -n' dan 'tail -n'. dimana 'n' head betambah 10 pada proses kedua. Hasil pengambilan list disimpan pada file SimpanProses#.txt pada folder home/Document/FolderProses#. Setelah itu, kita mengkompres file dengan 'system' 'zip -qmj'.
  - q agar tidak menampilkan promp message saat compress file, 
  - m agar file source zip dihapus, 
  - j agar dalam file zip tidak mengandung path direktori. 
setelah itu kita sleep program selama 15 detik. Kemudian kita ekstrak file dengan 'system' 'unzip -qd'.
  - q agar tidak menampilkan promp message, 
  - d untuk mengekstrak pada folder sesuai keinginan kita. 
