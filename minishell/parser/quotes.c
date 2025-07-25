// quotes.c içinde yer alabilecek potansiyel fonksiyonlar

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
