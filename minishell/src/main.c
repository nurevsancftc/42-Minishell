int main(int argc, char **argv, char **envp)
{
    // 1. Başlatma
    // - Ortam değişkenlerini kopyala ve yönet.
    // - Sinyalleri ayarla.
    // - Komut geçmişini başlat.

    while (1)
    {
        // 2. Komut Satırını Oku
        // - readline() ile kullanıcıdan girdi al.
        // - EOF (Ctrl+D) kontrolü yap.

        // 3. Ayrıştırma (Parsing)
        lexer()// - Lexer: Girdiyi token'lara ayır.
        parse_args() // - Parser: Token'lardan bir komut tablosu veya AST oluştur.

        // 4. Genişletme (Expansion)
        // - Değişkenleri ve tırnakları işle.

        // 5. Yürütme (Execution)
        // - Komutları çalıştır (yerleşik veya harici).
        // - Pipe ve yönlendirmeleri yönet.

        // 6. Temizlik
        // - Ayrılan belleği serbest bırak.
    }
    return (0);
}