// quotes.c içinde yer alabilecek potansiyel fonksiyonlar

/*
 * HANDLE_QUOTES FONKSİYONU
 * Görevi: Tek veya çift tırnakla başlayan bir bölümü işlemek.
 * 1. Başlangıç tırnağını (quote_char) kaydeder.
 * 2. Satırda ilerleyerek eşleşen kapanış tırnağını arar.
 * 3. Eğer satır sonuna gelinir ve kapanış tırnağı bulunamazsa, bu bir
 *    sözdizimi hatasıdır. Hata bildirilir ve -1 döndürülür.
 * 4. Kapanış tırnağı bulununca, tırnakların arasındaki metin (içerik) kopyalanır.
 * 5. Kopyalanan içerik ile yeni bir T_WORD token'ı oluşturulur.
 * 6. Bu token, ana token listesine eklenir.
 * 7. İşlenen toplam karakter sayısı (başlangıç ve bitiş tırnakları dahil) döndürülür.
*/





/**
 * @brief Bir token'ın kapatılmamış tırnak içerip içermediğini kontrol eder.
 * @return Hata varsa 1, yoksa 0.
 */
int	has_unclosed_quotes(char *str);

/**
 * @brief Bir token'daki değişkenleri genişletir ve tırnakları kaldırır.
 * @param token İşlenecek token (örneğin, "\"$USER\"").
 * @param env Ortam değişkenleri listesi.
 * @return İşlenmiş ve temizlenmiş yeni string (örneğin, "cihan").
 */
char *expand_and_remove_quotes(char *token, t_list *env);

/**
 * @brief Bir karakterin tırnak içinde olup olmadığını belirlemek için
 *        kullanılan bir state machine (durum makinesi) fonksiyonu.
 */
void update_quote_state(char current_char, int *quote_state); 
