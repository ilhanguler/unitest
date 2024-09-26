#pragma once
#include "YumurtaProblemi.h"

// Bu kaynak, yumurta problemini ele al�yor. 2'den fazla 64 bit yumurta say�s�n� ve 64 bitlik kat de�erlerinin hepsini i�liyor.
// Optimum yolla istenen de�ere h�zl�ca ula��yor.

// Problem de�i�ken tan�mlamalar�. Katlar� segmentlere b�l�yoruz.
UINT64 katSayisi = 0;     // Kullan�c� taraf�ndan belirlenen orijinal kat say�s�
//UINT katSayisiKalan = 0;// �ptal Edildi... Orijinal de�eri korumak i�in bir neden yok
UINT64 katTamBolunen = 0; // Segment boyutuna tam b�l�nen en y�ksek kat
UINT64 katBulunmasiIstenen = 0;
UINT64 katBulunan = 0;    // Sonuca yakla��rken al�nan toplam kat yolu
UINT64 katBulunanParca = 0; // �st�nde �al���lan par�ada gidilen yol. Bulunan de�erler, "katBulunan" de�i�keninde toplan�r.
UINT64 yumurtaSayisi = 0;
UINT64 yumurtaKirildi = 0;// {0,1}
UINT64 segmentGidilen = 0; // Segment se�imine gelen kadar i�lenen segment say�s�n� belirtir.
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
    // Kullan�c�dan girdi isteniyor.
    cout << "Var olan kat sayisini gir (0 = rastgele): ";
    cin >> katSayisi;
    cout << "Var olan yumurta sayisini gir (0 = rastgele): ";
    cin >> yumurtaSayisi;
    cout << "Bulunmasi istenen kat numarasini gir (0 = rastgele): ";
    cin >> katBulunmasiIstenen;

    // Rastgele de�erler istendiyse atan�yor
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
    // Yumurta say�s� 2'den b�y�k olabilece�i i�in fonksiyonlar� geni�letiyoruz.
    for (int parcalaraAyrildi = 1;; parcalaraAyrildi++)
    {
        // Segment hesaplamalar�.
        segmentBoyutu = round(sqrt(katSayisi));
        //segmentPaylasimi = segmentSayisiGidilen / round((yumurtaSayisi + 1) / 2);

        // Son segmentin boyutu ve tam b�l�nen en y�ksek kat, optimizason i�in hesaplan�yor
        segmentBoyutuSonuncu = (katSayisi - (segmentBoyutu - 1) * segmentBoyutu) % segmentBoyutu;
        katTamBolunen = katSayisi - segmentBoyutuSonuncu;

        // G�kdeleni segmentlere, segmentleri de ba�ka segmentlere b�lebilece�imiz i�in,
        // yap�lan i�lem hakk�nda bilgiler ekrana yaz�l�r.
        cout << endl << "Segment Boyutu: " << segmentBoyutu << endl << "Segment Sayisi: " << segmentGidilen
            << endl << "Son Segment Boyutu: " << segmentBoyutuSonuncu
            << endl << "Kat Sayisi: " << katSayisi << endl << "Yumurta Sayisi: " << yumurtaSayisi
            << endl << "Adim Sayisi: " << adimSayisi << endl << "Parcalara Ayrildi: " << parcalaraAyrildi << endl;

        // Segmentlerin s�ras�yla en �st kat�n� deniyoruz.
        for (; (katBulunanParca <= katBulunmasiIstenen) && (katBulunanParca < katTamBolunen); adimSayisi++) {
            katBulunanParca += segmentBoyutu;
            segmentGidilen++;
        }

        // Yumurta k�r�ld� m� yoksa tam b�l�nen kat say�s�na ula�t�k m� kontrol ediyoruz.
        if (katBulunanParca > katBulunmasiIstenen) {
            yumurtaKirildi = 1;
            yumurtaSayisi--;
        }

        // A��rtmay� d�zeltiyoruz ve gerekirse buldu�umuz segmentin ba��na d�n�yoruz.
        if (yumurtaKirildi)
        {
            katBulunanParca -= segmentBoyutu;
            segmentGidilen--;
            yumurtaKirildi = 0;
        }

        // Elimizde kalan yumurta say�s� 1 ise teker teker denemeye ba�l�yoruz. 
        // E�er de�il ise, segmentleri par�alara b�lmeye devam ediyoruz.
        // B�l�necek par�a ideal olarak 4'ten k���k ise k�k�n� almadan teker teker denemeye ba�l�yoruz.
        if (yumurtaSayisi == 1 || katSayisi < 16 || segmentBoyutu < 4)
        {
            // Bulunmas� istenen kat� bilmedi�imizi farz ediyoruz. O y�zden her ad�mda a��rtmay� kontrol ediyoruz.
            for (; (katBulunanParca <= katBulunmasiIstenen) && (katBulunanParca < katSayisi); adimSayisi++) {
                katBulunanParca++;
            }

            // Yumurta k�r�ld� m� yoksa en y�ksek kat say�s�na ula�t�k m� kontrol ediyoruz.
            if (katBulunanParca > katBulunmasiIstenen) {
                yumurtaKirildi = 1;
                yumurtaSayisi--;
            }

            // A��rtmay� d�zeltiyoruz.
            if (yumurtaKirildi)
            {
                katBulunanParca--;
                yumurtaKirildi = 0;
            }

            katBulunan += katBulunanParca;

            // ��lem sonucunda ana d�ng�den ��k�yoruz.
            break;
        }
        else
        {
            // Elimizde yumurtam�z kald�ysa veya kat say�m�z �ok k���k de�il ise, 
            // bulunan segmenti daha k���k par�alara b�lmek i�in de�er d�zeltmesi yap�l�r.
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
    // Sonu�lar� ekrana yazd�r�r�z.
    cout << endl << "SONUCLAR---------------------------------" << endl;
    cout << "*\tBulunmasi istenilen kati bulduk: " << katBulunan << endl;
    cout << "*\tAdim sayisi: " << adimSayisi << endl;
    cout << "*\tElde kalan yumurta: " << yumurtaSayisi << endl << endl;

    return 1;
}