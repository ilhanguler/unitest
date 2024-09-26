#pragma once
#include "YumurtaProblemi.h"

// Bu kaynak, yumurta problemini ele alýyor. 2'den fazla 64 bit yumurta sayýsýný ve 64 bitlik kat deðerlerinin hepsini iþliyor.
// Optimum yolla istenen deðere hýzlýca ulaþýyor.

// Problem deðiþken tanýmlamalarý. Katlarý segmentlere bölüyoruz.
UINT64 katSayisi = 0;     // Kullanýcý tarafýndan belirlenen orijinal kat sayýsý
//UINT katSayisiKalan = 0;// Ýptal Edildi... Orijinal deðeri korumak için bir neden yok
UINT64 katTamBolunen = 0; // Segment boyutuna tam bölünen en yüksek kat
UINT64 katBulunmasiIstenen = 0;
UINT64 katBulunan = 0;    // Sonuca yaklaþýrken alýnan toplam kat yolu
UINT64 katBulunanParca = 0; // Üstünde çalýþýlan parçada gidilen yol. Bulunan deðerler, "katBulunan" deðiþkeninde toplanýr.
UINT64 yumurtaSayisi = 0;
UINT64 yumurtaKirildi = 0;// {0,1}
UINT64 segmentGidilen = 0; // Segment seçimine gelen kadar iþlenen segment sayýsýný belirtir.
UINT64 segmentBoyutu = 0;
UINT64 segmentBoyutuSonuncu = 0; // Son segmentin boyutu
UINT64 adimSayisi = 0;


void rastgeleDeger(UINT64& deger, int aralik) {
    srand(time(NULL));
    deger = rand() % aralik;
}
void rastgeleDeger(int& deger, int aralik) {
    srand(time(NULL));
    deger = rand() % aralik;
}

int yumurtaGirdi() {
    // Kullanýcýdan girdi isteniyor.
    cout << "Var olan kat sayisini gir (0 = rastgele): ";
    cin >> katSayisi;
    cout << "Var olan yumurta sayisini gir (0 = rastgele): ";
    cin >> yumurtaSayisi;
    cout << "Bulunmasi istenen kat numarasini gir (0 = rastgele): ";
    cin >> katBulunmasiIstenen;

    // Rastgele deðerler istendiyse atanýyor
    srand(time(0));
    if (katSayisi == 0)
    {
        rastgeleDeger(katSayisi, 100000);
    }

    if (yumurtaSayisi == 0)
    {
        rastgeleDeger(yumurtaSayisi, 100);
    }

    if (katBulunmasiIstenen == 0)
    {
        rastgeleDeger(katBulunmasiIstenen, katSayisi);
    }

    return 1;
}

int yumurtaCalistir() {
    // Yumurta sayýsý 2'den büyük olabileceði için fonksiyonlarý geniþletiyoruz.
    for (int parcalaraAyrildi = 1;; parcalaraAyrildi++)
    {
        // Segment hesaplamalarý.
        segmentBoyutu = round(sqrt(katSayisi));
        //segmentPaylasimi = segmentSayisiGidilen / round((yumurtaSayisi + 1) / 2);

        // Son segmentin boyutu ve tam bölünen en yüksek kat, optimizason için hesaplanýyor
        segmentBoyutuSonuncu = (katSayisi - (segmentBoyutu - 1) * segmentBoyutu) % segmentBoyutu;
        katTamBolunen = katSayisi - segmentBoyutuSonuncu;

        // Gökdeleni segmentlere, segmentleri de baþka segmentlere bölebileceðimiz için,
        // yapýlan iþlem hakkýnda bilgiler ekrana yazýlýr.
        cout << endl << "Segment Boyutu: " << segmentBoyutu << endl << "Segment Sayisi: " << segmentGidilen
            << endl << "Son Segment Boyutu: " << segmentBoyutuSonuncu
            << endl << "Kat Sayisi: " << katSayisi << endl << "Yumurta Sayisi: " << yumurtaSayisi
            << endl << "Adim Sayisi: " << adimSayisi << endl << "Parcalara Ayrildi: " << parcalaraAyrildi << endl;

        // Segmentlerin sýrasýyla en üst katýný deniyoruz.
        for (; (katBulunanParca <= katBulunmasiIstenen) && (katBulunanParca < katTamBolunen); adimSayisi++) {
            katBulunanParca += segmentBoyutu;
            segmentGidilen++;
        }

        // Yumurta kýrýldý mý yoksa tam bölünen kat sayýsýna ulaþtýk mý kontrol ediyoruz.
        if (katBulunanParca > katBulunmasiIstenen) {
            yumurtaKirildi = 1;
            yumurtaSayisi--;
        }

        // Aþýrtmayý düzeltiyoruz ve gerekirse bulduðumuz segmentin baþýna dönüyoruz.
        if (yumurtaKirildi)
        {
            katBulunanParca -= segmentBoyutu;
            segmentGidilen--;
            yumurtaKirildi = 0;
        }

        // Elimizde kalan yumurta sayýsý 1 ise teker teker denemeye baþlýyoruz. 
        // Eðer deðil ise, segmentleri parçalara bölmeye devam ediyoruz.
        // Bölünecek parça ideal olarak 4'ten küçük ise kökünü almadan teker teker denemeye baþlýyoruz.
        if (yumurtaSayisi == 1 || katSayisi < 16 || segmentBoyutu < 4)
        {
            // Bulunmasý istenen katý bilmediðimizi farz ediyoruz. O yüzden her adýmda aþýrtmayý kontrol ediyoruz.
            for (; (katBulunanParca <= katBulunmasiIstenen) && (katBulunanParca < katSayisi); adimSayisi++) {
                katBulunanParca++;
            }

            // Yumurta kýrýldý mý yoksa en yüksek kat sayýsýna ulaþtýk mý kontrol ediyoruz.
            if (katBulunanParca > katBulunmasiIstenen) {
                yumurtaKirildi = 1;
                yumurtaSayisi--;
            }

            // Aþýrtmayý düzeltiyoruz.
            if (yumurtaKirildi)
            {
                katBulunanParca--;
                yumurtaKirildi = 0;
            }

            katBulunan += katBulunanParca;

            // Ýþlem sonucunda ana döngüden çýkýyoruz.
            break;
        }
        else
        {
            // Elimizde yumurtamýz kaldýysa veya kat sayýmýz çok küçük deðil ise, 
            // bulunan segmenti daha küçük parçalara bölmek için deðer düzeltmesi yapýlýr.
            katBulunan += katBulunanParca;
            katBulunanParca = 0;
            katSayisi = segmentBoyutu;
            katBulunmasiIstenen -= segmentBoyutu * segmentGidilen;
            segmentGidilen = 0;
        }


    }

    return 1;
}

int yumurtaSonuclar() {
    // Sonuçlarý ekrana yazdýrýrýz.
    cout << endl << "SONUCLAR---------------------------------" << endl;
    cout << "*\tBulunmasi istenilen kati bulduk: " << katBulunan << endl;
    cout << "*\tAdim sayisi: " << adimSayisi << endl;
    cout << "*\tElde kalan yumurta: " << yumurtaSayisi << endl << endl;

    return 1;
}