#include <bits/stdc++.h>

using namespace std;

void carregarRegistradores(fstream& Registradores, int32_t& MAR, int32_t& MDR, int32_t& PC, uint8_t& MBR, int32_t& SP, int32_t& LV, int32_t& CPP, int32_t& TOS, int32_t& OPC, int32_t& H) {
        string linha;
        map<string, int32_t> valores;

        while (getline(Registradores, linha)) {
                int pos = linha.find('=');
                if (pos == string::npos)
                        continue;
                string nome = linha.substr(0, pos);
                string valorBinario = linha.substr(pos + 1);

                // Remover espaços
                nome.erase(remove_if(nome.begin(), nome.end(), ::isspace), nome.end());
                valorBinario.erase(remove_if(valorBinario.begin(), valorBinario.end(), ::isspace), valorBinario.end());

                // Transforma p/ aceitar o binario
                valores[nome] = stoi(valorBinario, nullptr, 2);
        }

        // Atribuindo os valores aos registradores
        // Observação: se o arquivo não contiver alguma chave, o map retorna 0 (comportamento desejado).
        MAR = valores["mar"];
        MDR = valores["mdr"];
        PC = valores["pc"];
        MBR = static_cast<uint8_t>(valores["mbr"]); // mantemos somente 8 bits por casting
        SP = valores["sp"];
        LV = valores["lv"];
        CPP = valores["cpp"];
        TOS = valores["tos"];
        OPC = valores["opc"];
        H = valores["h"];

        Registradores.close();
}

/**
 * @brief Simula a ULA (Unidade Lógica e Aritmética) de acordo com os sinais de controle. (cumpre etapa 1)
 *
 * @param F0,F1 Seleção da operação lógica/aritmética
 * @param INVA Inverte A caso habilitado
 * @param ENA Habilita entrada A
 * @param ENB Habilita entrada B
 * @param INC Carry-in (bit de entrada da soma)
 * @param A, B Entradas da ULA
 * @param S Saída da ULA
 * @param VAI Carry-out (vai-um)
 * Comentários:
 * - Para a soma usamos uint64_t para detectar overflow de 32 bits e computar VAI com segurança.
 * - A inversão de A com INVA faz ~A antes de qualquer soma/operacao.
 * - O interpretador das operações segue as escolhas originais do código.
 */
void decodificadorSinaisULA(int F0, int F1, int INVA, int ENA, int ENB, int INC, int32_t A, int32_t B, int32_t& S, int& VAI) {

        // Variáveis locais para armazenar as entradas de A e B após processamento dos sinais
        int32_t entrada_A = 0;
        int32_t entrada_B = 0;

        // Verifica se a entrada A está habilitada, caso esteja, aplica a inversão se necessário
        if (ENA) {
                entrada_A = INVA ? ~A : A;
        }

        // Verifica se a entrada B está habilitada, caso esteja, atribui o valor de B
        if (ENB) {
                entrada_B = B;
        }

        S = 0;
        VAI = 0; // Saídas da ULA inicializadas, Vai-um (carry out) e S (resultado, sem deslocamento)

        // Decodifica a operação a ser realizada com base nos sinais F0 e F1
        if (!F0 && !F1) { // 00
                // A & B (AND)
                S = (entrada_A & entrada_B);
        } else if (!F0 && F1) { // 01
                // A + B (OR)
                S = (entrada_A | entrada_B);
        } else if (F0 && !F1) { // 10
                // ~B (NOT B)
                S = ~entrada_B;
        } else if (F0 && F1) { // 11
                // Soma com carry-in
                uint64_t soma = (uint64_t)(uint32_t)entrada_A + (uint64_t)(uint32_t)entrada_B + (uint64_t)INC;
                S = (int32_t)soma;
                VAI = (soma > 0xFFFFFFFF); // Define o Vai-um (carry out)
        }
}

/**
 * @brief Seleciona qual registrador irá colocar valor no barramento B e, assim, representar o operando B da ULA. (etapa 2 tarefa 2)
 * Codificação (enumeração) de acordo com as especificações do trabalho
 */
int32_t decodificadorBarramentoB(uint8_t codigoB, int32_t MDR, int32_t PC, uint8_t MBR, int32_t SP, int32_t LV, int32_t CPP, int32_t TOS, int32_t OPC) {
        switch (codigoB) {
        case 0:
                return MDR;

        case 1:
                return PC;

        case 2:
                return static_cast<int32_t>(static_cast<int8_t>(MBR));

        case 3:
                return static_cast<int32_t>(MBR);

        case 4:
                return SP;

        case 5:
                return LV;

        case 6:
                return CPP;

        case 7:
                return TOS;

        default:
                return OPC;
        }
}

/**
 * @brief Retorna o nome do registrador que está no barramento B. (etapa 2 tarefa 2) - Mesmo registrador da função acima
 * Codificação (enumeração) de acordo com as especificações do trabalho
 */
string nomeDoRegistradorNoBarramentoB(uint8_t codigoB) {
        switch (codigoB) {
        case 0:
                return "MDR";

        case 1:
                return "PC";

        case 2: // case 2: MBR com sinal (extensão de sinal para 32 bits)
                return "MBR";

        case 3: // case 3: MBR sem sinal (zero-extend)
                return "MBRU";

        case 4:
                return "SP";

        case 5:
                return "LV";

        case 6:
                return "CPP";

        case 7:
                return "TOS";

        default:
                return "OPC";
        }
}

/**
 * @brief Retorna os nomes dos registradores que estão sendo escritos pelo barramento C.
 * segue a decodificação da especificação do trabalho, sendo: cada bit corresponde a um registrador
 */
string nomesDosRegistradoresC(const array<int, 23>& instrucao) {
        vector<string> nomes = {"H", "OPC", "TOS", "CPP", "LV", "SP", "PC", "MDR", "MAR"};
        string resultado;
        bool primeiro = true;

        for (size_t i = 0; i < nomes.size(); ++i) {
                // Verifica o bit correspondente (índices 8 a 16)
                if (instrucao[8 + i]) {
                        if (!primeiro) {
                                resultado += ", ";
                        }
                        resultado += nomes[i];
                        primeiro = false;
                }
        }

        return resultado.empty() ? "Nenhum" : resultado;
}

/**
 * @brief Escreve o valor Sd (resultado deslocado) nos registradores habilitados (barramento C).
 * segue a decodificação da especificação do trabalho, sendo: cada bit corresponde a um registrador
 */
void seletorBarramentoC(std::array<int, 23> instrucao, int32_t Sd,
                        int32_t& H, int32_t& OPC, int32_t& TOS, int32_t& CPP,
                        int32_t& LV, int32_t& SP, int32_t& PC, int32_t& MDR, int32_t& MAR) {

        if (instrucao[8])
                H = Sd; // bit 8 - H
        if (instrucao[9])
                OPC = Sd; // bit 7 - OPC
        if (instrucao[10])
                TOS = Sd; // bit 6 - TOS
        if (instrucao[11])
                CPP = Sd; // bit 5 - CPP
        if (instrucao[12])
                LV = Sd; // bit 4 - LV
        if (instrucao[13])
                SP = Sd; // bit 3 - SP
        if (instrucao[14])
                PC = Sd; // bit 2 - PC
        if (instrucao[15])
                MDR = Sd; // bit 1 - MDR
        if (instrucao[16])
                MAR = Sd; // bit 0 - MAR
}

/**
 * @brief Converte um inteiro em uma string binária de 32 bits.
 *
 * Esta função gera a representação binária de 32 bits de um número inteiro, retornando uma string
 * contendo apenas os caracteres '0' e '1'.
 *
 * @param valor O valor inteiro a ser convertido.
 *
 * @return Uma string com 32 caracteres representando o valor binário de `valor`.
 */
string conversor_binario(int valor) {
        string s = "";
        for (int i = 0; i < 32; i++) {
                if ((valor & (1 << i))) {
                        s.push_back('1');
                } else
                        s.push_back('0');
        }

        reverse(s.begin(), s.end());

        return s;
}

/**
 * @brief Abre o arquivo e lê a memória inicial de tokens de 32 bits
 * converte cada string de binário pra int32 e coloca no vetor MEM
 */
bool lerMemoria32Bin(const string& path, vector<int32_t>& MEM) {
        ifstream fin(path);
        if (!fin) {
                return false;
        }

        MEM.clear();
        string line;

        while (fin >> line) {
                if (line.size() == 32) { // ignora linhas que estejam invalidas
                        MEM.push_back(stoll(line, nullptr, 2));
                }
        }
        return true;
}

/**
 * @brief Converte string de 23 bits em microinstrução (array<int, 23>).
 */
array<int, 23> string_para_microinstrucao(const string& s) {
        array<int, 23> micro;
        for (size_t i = 0; i < 23; ++i) {
                micro[i] = s[i] - '0';
        }
        return micro;
}

/**
 * @brief Executa uma sequência de microinstruções (vector de array<23>) atualizando o estado dos registradores e MEM.
 *
 * Parâmetros principais:
 * - micro_instrucoes: sequência de microinstruções (cada uma com 23 bits)
 * - registradores por referência: H, OPC, TOS, CPP, LV, SP, PC_registrador, MDR, MAR, MBR
 * - MEM: vetor de memória de dados (lido previamente)
 * - Saida: ofstream aberto para gravar logs (detalhado)
 * - ciclo_global: int de contagem global de microinstruções (incrementado por referência)
 *
 * Comportamento:
 * - Para cada microinstrução gera logs: instrução binária, registradores no início, nome do registrador
 *   no barramento B, registradores a serem habilitados no barramento C, etc.
 * - Implementa o caso especial "FETCH" de BIPUSH: detectado quando READ && WRITE são ambos 1.
 *   Nesse caso, os primeiros 8 bits da microinstrução são interpretados como o byte parameter.
 * - Caso normal:
 *     * Determina A <- H, B <- valor do registrador selecionado por decodificadorBarramentoB.
 *     * Chama decodificadorSinaisULA com os sinais apropriados.
 *     * Ajusta Sd conforme bits de shift (instrucao[0] e instrucao[1]).
 *     * Chama seletorBarramentoC para escrever Sd nos registradores selecionados.
 *     * Se READ = 1: carrega MDR <- MEM[MAR] (verifica MAR válido)
 *     * Se WRITE = 1: escreve MEM[MAR] <- MDR (verifica MAR válido)
 *
 */
void executar_microinstrucoes(vector<array<int, 23>>& micro_instrucoes,
                              int32_t& H, int32_t& OPC, int32_t& TOS, int32_t& CPP, int32_t& LV, int32_t& SP, int32_t& PC_registrador, int32_t& MDR, int32_t& MAR, uint8_t& MBR,
                              vector<int32_t>& MEM,
                              ofstream& Saida, int& ciclo_global) {

        for (const auto& instrucao_atual : micro_instrucoes) {
                Saida << "--- Microinstrução " << ciclo_global++ << " ---" << endl;
                string instrucao_str;
                for (int bit : instrucao_atual) instrucao_str += to_string(bit);
                Saida << "Instrução (IR): " << instrucao_str << endl;

                Saida << "Registradores no Início:" << endl;
                Saida << "H = " << H << " OPC = " << OPC << " TOS = " << TOS << " CPP = " << CPP << " LV = " << LV << " SP = " << SP << " PC = " << PC_registrador << " MDR = " << MDR << " MAR = " << MAR << " MBR = " << (int)MBR << endl;

                // Decodifica e executa exatamente como antes
                int WRITE = instrucao_atual[17];
                int READ = instrucao_atual[18];
                uint8_t codigoB = (instrucao_atual[19] << 3) | (instrucao_atual[20] << 2) | (instrucao_atual[21] << 1) | (instrucao_atual[22]);

                Saida << "Barramento B comandado por: " << nomeDoRegistradorNoBarramentoB(codigoB) << endl;
                Saida << "Barramento C habilitado para: " << nomesDosRegistradoresC(instrucao_atual) << " | READ = " << READ << " WRITE = " << WRITE << endl;

                // --- INÍCIO DO CICLO DE EXECUÇÃO ---

                // CASO ESPECIAL DO FETCH PARA BIPUSH
                if (READ && WRITE) {
                        Saida << "[FETCH] Carregando byte para MBR e H" << endl;
                        uint8_t byte_param = 0;
                        for (int i = 0; i < 8; ++i) {
                                byte_param = (byte_param << 1) | instrucao_atual[i];
                        }
                        MBR = byte_param;
                        H = static_cast<uint32_t>(MBR); // Preenchimento com zeros
                } else {
                        // Lógica normal
                        int32_t A = H;
                        int32_t B = decodificadorBarramentoB(codigoB, MDR, PC_registrador, MBR, SP, LV, CPP, TOS, OPC);
                        int32_t S;
                        int VAI = 0;
                        decodificadorSinaisULA(instrucao_atual[2], instrucao_atual[3], instrucao_atual[6], instrucao_atual[4], instrucao_atual[5], instrucao_atual[7], A, B, S, VAI);

                        int32_t Sd = S;
                        if (instrucao_atual[0])
                                Sd <<= 8;
                        if (instrucao_atual[1])
                                Sd >>= 1;

                        seletorBarramentoC(instrucao_atual, Sd, H, OPC, TOS, CPP, LV, SP, PC_registrador, MDR, MAR);

                        if (READ && !WRITE) {
                                Saida << "[READ] MDR <- MEM[" << MAR << "]";
                                if (MAR >= 0 && MAR < (int)MEM.size()) {
                                        Saida << "(" << MEM[MAR] << ")" << endl;
                                        MDR = MEM[MAR];
                                } else {
                                        Saida << endl
                                              << "[READ][ERRO] Endereço MAR inválido: " << MAR << endl;
                                }
                        }

                        if (WRITE && !READ) {
                                Saida << "[WRITE] MEM[" << MAR << "] <- MDR(" << MDR << ")" << endl;
                                if (MAR >= 0 && MAR < (int)MEM.size()) {
                                        MEM[MAR] = MDR;
                                } else {
                                        Saida << "[WRITE][ERRO] Endereço MAR inválido: " << MAR << endl;
                                }
                        }
                }

                Saida << "Registradores no Fim:" << endl;
                Saida << "H = " << H << " OPC = " << OPC << " TOS = " << TOS << " CPP = " << CPP << " LV = " << LV << " SP = " << SP << " PC = " << PC_registrador << " MDR = " << MDR << " MAR = " << MAR << " MBR = " << (int)MBR << endl
                      << endl;

                Saida << "Memória de Dados:" << endl;
                for (size_t i = 0; i < MEM.size(); ++i) {
                        Saida << "MEM[" << i << "] = " << conversor_binario(MEM[i]) << endl;
                }
                Saida << endl;
        }
}

int main() {
        // Resumo do fluxo principal:
        // 1) Carrega memória de dados (memoria_inicial.txt).
        // 2) Lê arquivo de registradores iniciais (registrador_inicial.txt).
        // 3) Lê instruções de alto nível (instrucoes.txt) e traduz para microinstruções.
        // 4) Executa microinstruções geradas e grava logs em saida_etapa3_tarefa1.txt.

        // --- CONFIGURAÇÃO DOS ARQUIVOS ---

        // As instruções virão de um arquivo
        string caminho_instrucoes_alto_nivel = "instrucoes.txt";
        // Os dados virão de um arquivo
        string caminho_dados = "memoria_inicial.txt";
        // O estado inicial dos registradores virá de um arquivo
        string caminho_registradores = "registrador_inicial.txt";
        // O resultado sairá em um arquivo
        string caminho_saida = "saida.txt";

        // --- INICIALIZAÇÃO ---
        vector<int32_t> MEM;
        if (!lerMemoria32Bin(caminho_dados, MEM)) {
                // Adiciona um tratamento de erro, caso o arquivo não seja encontrado
                cerr << "ERRO: Não foi possível ler o arquivo de dados da memória: " << caminho_dados << endl;
                return 1; // Encerra o programa se a memória não puder ser carregada
        }

        fstream Registradores("registrador_inicial.txt");
        int32_t H = 0, OPC = 0, TOS = 0, CPP = 0, LV = 0, SP = 0, MDR = 0, MAR = 0;
        uint8_t MBR = 0;
        int32_t PC_registrador = 0;

        carregarRegistradores(Registradores, MAR, MDR, PC_registrador, MBR, SP, LV, CPP, TOS, OPC, H);

        ifstream MaquinaVirtual(caminho_instrucoes_alto_nivel);
        if (!MaquinaVirtual) {
                cerr << "ERRO: Não foi possível ler o arquivo de instruções " << caminho_instrucoes_alto_nivel << endl;
                return 1;
        }

        ofstream Saida(caminho_saida);
        string linha_instrucao;
        int ciclo_global = 1;

        // --- DEFINIÇÃO DAS MICROINSTRUÇÕES ---
        const auto H_EQ_LV = string_para_microinstrucao("00010100100000000000101");
        const auto H_EQ_H_MAIS_1 = string_para_microinstrucao("00111001100000000000000");
        const auto MAR_EQ_H_RD = string_para_microinstrucao("00111000000000001010000");
        const auto MAR_SP_EQ_SP_MAIS_1_WR = string_para_microinstrucao("00110101000001001100100");
        const auto TOS_EQ_MDR = string_para_microinstrucao("00110100001000000000000");
        const auto MAR_SP_EQ_SP_MAIS_1 = string_para_microinstrucao("00110101000001001000100");
        const auto MDR_EQ_TOS_WR = string_para_microinstrucao("00010100000000010100111");
        const auto MDR_TOS_EQ_H_WR = string_para_microinstrucao("00111000001000010100000");

        // Lê todas as instruções (linhas) do arquivo de entrada
        while (getline(MaquinaVirtual, linha_instrucao)) {
                stringstream ss(linha_instrucao);
                string comando;
                ss >> comando;

                vector<array<int, 23>> micro_instrucoes_para_executar;

                Saida << "========================================================" << endl;
                Saida << "EXECUTANDO INSTRUÇÃO DE ALTO NÍVEL: " << linha_instrucao << endl;
                Saida << "========================================================" << endl;

                if (comando == "ILOAD") {
                        int x;
                        ss >> x;
                        // Traduzir ILOAD x
                        // Dinamicidade, x > 0 produz mais microinstruções do tipo H = H + 1
                        micro_instrucoes_para_executar.push_back(H_EQ_LV);
                        for (int i = 0; i < x; ++i) {
                                micro_instrucoes_para_executar.push_back(H_EQ_H_MAIS_1);
                        }
                        micro_instrucoes_para_executar.push_back(MAR_EQ_H_RD);
                        micro_instrucoes_para_executar.push_back(MAR_SP_EQ_SP_MAIS_1_WR);
                        micro_instrucoes_para_executar.push_back(TOS_EQ_MDR);

                } else if (comando == "DUP") {
                        // Traduzir DUP
                        micro_instrucoes_para_executar.push_back(MAR_SP_EQ_SP_MAIS_1);
                        micro_instrucoes_para_executar.push_back(MDR_EQ_TOS_WR);

                } else if (comando == "BIPUSH") {
                        string byte_arg;
                        ss >> byte_arg; // Lê o argumento como string

                        // Validação para garantir que o argumento é um binário válido de até 8 bits
                        if (byte_arg.length() > 8 || byte_arg.find_first_not_of("01") != string::npos) {
                                cerr << "ERRO: Argumento inválido para BIPUSH. Deve ser um binário de até 8 bits. Argumento fornecido: " << byte_arg << endl;
                                Saida << "!!!! ERRO: Argumento inválido para BIPUSH: " << byte_arg << " !!!!\n";
                                continue; // Pula para a próxima instrução
                        }

                        // Adiciona zeros à esquerda se o comprimento for menor que 8
                        while (byte_arg.length() < 8) {
                                byte_arg = "0" + byte_arg;
                        }

                        micro_instrucoes_para_executar.push_back(MAR_SP_EQ_SP_MAIS_1);

                        string fetch_micro_str = byte_arg + "000000000110000";
                        micro_instrucoes_para_executar.push_back(string_para_microinstrucao(fetch_micro_str));

                        micro_instrucoes_para_executar.push_back(MDR_TOS_EQ_H_WR);
                }

                // 3. EXECUÇÃO DA SEQUÊNCIA GERADA
                if (!micro_instrucoes_para_executar.empty()) {
                        executar_microinstrucoes(micro_instrucoes_para_executar, H, OPC, TOS, CPP, LV, SP, PC_registrador, MDR, MAR, MBR, MEM, Saida, ciclo_global);
                }

                PC_registrador++;
        }

        Saida.close();
        MaquinaVirtual.close();
        cout << "Simulação finalizada. Verifique o arquivo" << endl;
}