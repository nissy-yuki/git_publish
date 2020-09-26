#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void drow(int x,int yama[][4],int hand[][2]){ //ドローの処理
    int  mark, num;
    while(1){
        num = rand() % 13;
        mark = rand() % 4;
        if(yama[num][mark] == 0){            //すでに使われた札を引いた場合もう一度引き直す
            continue;
        }
        hand[x][0] = num + 1;
        hand[x][1] = mark;
        yama[num][mark] = 0;                 //一度引いた札に0入れもう一度引かれても引き直せるようにする
        break;
    }
    return;
}

void shuffle(int yama[][4]){                 //山札を初期化
    for(int i = 0;i < 13;i++){      
        for(int j = 0;j < 4;j++){
            yama[i][j] =i + 1;               //i+1はそのまま数字、jが0はクローバー、1はダイヤ、2はハート、3はスペードを表す
        }
    }
    return;
}

void open(int mai,int hand[][2]){           //カードの表示
    int huda;
    for(huda = 0;huda < mai;huda++){
        printf(" ______");                  //札の上の線
    }
    printf("\n");
    for(huda = 0;huda < mai;huda++){
        printf(" |");
        switch(hand[huda][1]){       //札のマークを判定、表示
            case 0:                 //マークが環境依存文字しかなかったため、頭文字で代用
                putchar('C');       //クローバー
                break;
            case 1:
                putchar('D');       //ダイヤ
                break;
            case 2:
                putchar('H');       //ハート
                break;
            case 3:
                putchar('S');       //スペード
                break;
        }
        printf("%2d |", hand[huda][0]);  //札の数字を表示
    }
    printf("\n");
    return;
}

void trash(int sute[][2],int su){                    //捨て札の表示
    int set = su / 5;
    while(0 < set){
        open(5,sute);
        for(int a = 0;a < 49;a++){            //表示した捨て札の削除、残りの札を前につめる
            sute[a][0] = sute[a + 5][0];
            sute[a + 5][0] = 0;
            sute[a][1] = sute[a + 5][1];
        }
        set--;
    }
    su = su % 5;
    open(su,sute);
    return; 
}

void yaku(int point){                        //役の出力
    switch((point - 1) / 13){
        case 0:
            printf("ノーペア！\n");
            break;
        case 1:
            printf("ワンペア！\n");
            break;
        case 2:
            printf("ツーペア！\n");
            break;
        case 3:
            printf("スリーカード！\n");
            break;
        case 4:
            printf("ストレート！\n");
            break;
        case 5:
            printf("フラッシュ！\n");
            break;
        case 6:
            printf("フルハウス！\n");
            break;
        case 7:  
            printf("フォーカード！\n");
            break;
        case 8:                             //ストレートフラッシュの場合最大の加算が15のため
        case 9:
            printf("ストレートフラッシュ！\n");
            break;
        case 10:
            printf("ロイヤルストレートフラッシュ！\n");
            break;
    }
    return;
}

void save(int hand[][2],int keep[][2]){
    for(int s = 0;s < 5;s++){
        keep[s][0] = hand[s][0];
        keep[s][1] = hand[s][1];
    }
    return;
}

void sort(int hand[][2]){
    int tmp;
    for(int k = 0;k < 4;k++){
        for(int l = k + 1;l < 5;l++){
            if(hand[k][0] < hand[l][0]){  //手札を降順に並び替える
                tmp = hand[k][0];         
                hand[k][0] = hand[l][0];  //数字を入れ替える
                hand[l][0] = tmp;
                tmp = hand[k][1];
                hand[k][1] = hand[l][1];  //マークを入れ替える
                hand[l][1] = tmp;
            }
            if(hand[k][0] == hand[l][0] && hand[k][1] > hand[l][1]){  //もし札の数字が同じだった場合にはマークごとに並び替える
                tmp = hand[k][1];
                hand[k][1] = hand[l][1];
                hand[l][1] = tmp;
            }
        }
    }
    return;
}

int judge(int hand[][2]){       //役の判定を行う
    int p = 0;
    int pair[3] = {0,0,0};      //ペアができている枚数
    int rank[3] = {0,0,0};      //ペアができている札の数字
    int su = 0;                 //ペアの数
    int flash = 1;              //フラッシュが成り立っているか否か
    int straight = 1;           //ストレートが成り立っているか否か
    for(int m = 0;m < 4;m++){   //ペアの探索
        int n = m + 1;
        while(hand[m][0] == hand[n][0]){
            if(rank[su] != hand[m][0]){     //ペアが見つかった上で、いままで数えていたペアとは違う場合の処理                su++;                       //見つけたペア数を一つ増やす
                su++;                       //見つけたペア数を一つ増やす
                rank[su] = hand[m][0];      //見つけたペアの数字を登録
            }
            pair[su]++;                     //ペアの数を＋1
            n++;
            flash = 0;
            straight = 0;
        }
        if(hand[m][1] != hand[n][1]){       //違うマークが見つかった場合フラッシュを除外
                flash = 0;
        }
        if(hand[m][1] - hand[n][1] != 1){   //数字が連続していなかった場合ストレートを除外
            straight = 0;
        }
        m = n - 1;
    }
    switch(pair[1] + pair[2]){          //ペア数を数えている配列の要素を足す
        case 1:                             //ワンペアの場合は1番目の配列しか入っていないため
            p = 13 + rank[1];
            break;
        case 2:                             
            if(pair[1] == 2){               //スリーカード
                p = 39 + rank[1];
            }else{                          //ツーペア
                p = 26 + rank[1];
            }
            break;
        case 3:
            if(pair[1] == 3){               //フォーカード
                p = 91 + pair[1];
            }else{
                if(pair[2] > pair[1]){      //フルハウス
                    p = 78 + rank[2];
                }else{
                    p = 78 + rank[1];
                }
            }
            break;
        default:
            break;
    }                       //それぞれの役の重みに、札の数字の値を足すことで同じ役でも強弱をつけることができる
    if(hand[0][0] + hand[1][0] +hand[2][0] + hand[3][0] + hand[4][0] == 47 && hand[4][0] == 1 && flash == 1){
        p = 1;
        straight = 1;       //連番ではないため除外されているストレートを元に戻す
    }
    p += straight * (hand[0][0] + 52);
    p += flash * (hand[0][1] + 66);  //フラッシュの重みをつけているマークは0から数字が始まるので本来の重みに＋1をする
    return (p);
}

int get(int point){            //mypointからどの役かを導き、役の難しさによって倍率を変える
    int b;
    switch ((point - 1) / 13)   //全ての役のポイントは、数字＋13＊役の重みによって計算されているため、-1と/13をすることですることで役の強さがわかる
    {
    case 0:
        b = 1;
        break;
    case 1:
        b = 1;
        break;
    case 2:
        b = 2;
        break;
    case 3:
        b = 3;
        break;
    case 4:
        b = 4;
        break;
    case 5:
        b = 5;
        break;
    case 6:
        b = 7;
        break;
    case 7:  
        b = 8;
        break;
    case 8:                             //ストレートフラッシュの場合最大の加算が15のため
    case 9:
        b = 10;
        break;
    case 10:
        b = 20;
        break;
    }
    return (b);
}

void enemy(int yama[][4],int hand[][2],int hard){       //相手のターンの処理
    int change = 0;                                     //交換するカードの枚数
    int chand[5][2] = {{0,0},{0,0},{0,0},{0,0},{0,0}};  //交換する手札の判定
    int u;                                              //ループ用の変数
    int sh = 1;                                         //フラッシュとストレートの判定
    //以下はつよつよ用
    int te[10][2];                                      //手札の確認
    int maxpoint = 0;                                   //最も強い役の判定用
    int saituyo[5][2];                                  //最も強い場合の手札
    int nowpoint;
    int kari[5][2];                                     //判定用の仮手札
    int mi[] = {0,0,0,0,0};                             //kohoから交換するか否か
    switch(hard){
        case 1:                                         //弱いCPU、全てがランダム
            for(u = 0;u < 5;u++){
                int shin = rand() % 2;                      //randを使い交換するか否かを決める
                if(shin == 1){
                    drow(u,yama,hand);
                    change++;
                }
            }
            break;
        case 2:                                         //普通のCPU、ペアやストレート、フラッシュ以外を交換
            for(u = 0;u < 4;u++){                   //手札の探索
                int p = u + 1;
                while(hand[u][0] == hand[p][0]){
                    chand[u][0] = hand[u][0];           //手札をchandに入力
                    chand[u][1] = hand[u][1];
                    chand[p][0] = hand[p][0];
                    chand[p][1] = hand[p][1];
                    u++;
                    p++;
                }
                if(hand[u][1] != hand[p][1] || hand[u][1] - hand[p][1] != 1){       //違うマークや連続した数字以外が見つかった場合ストレート、フラッシュを除外
                    sh = 0;
                }
                u = p - 1;
            }
            if(sh == 0){
                for(u = 0;u < 5;u++){
                    int hum = rand() % 3;              //人間っぽくしたいのでrandを使い交換するか否かを判断させる
                    if(chand[u][0] == 0 && hum != 0){  //ペアになっていないカードを交換する
                        drow(u,yama,hand);
                        change++;
                    }
                }
            }
            break;
        case 3:                                       //強いCPU、つよつよ(イカサマ)
            for(u = 0;u < 5;u++){
                drow(u,yama,chand);
            }
            sort(chand);
            for(u = 0;u < 10;u++){                 //teの初期化
                te[u][0] = 0;
                te[u][1] = 0;
            }
            for(u = 0;u < 5;u++){                  //teにhandとkohoを入れる
                te[u][0] = hand[u][0];
                te[u][1] = hand[u][1];
                te[u + 5][0] = chand[u][0];
                te[u + 5][1] = chand[u][1];
            }
            for(int u = 0;u < 9;u++){
                int tmp;
                for(int v = u + 1;v < 10;v++){
                    if(te[u][0] < te[v][0]){  //手札を降順に並び替える
                        tmp = te[u][0];         
                        te[u][0] = te[v][0];  //数字を入れ替える
                        te[v][0] = tmp;
                        tmp = te[u][1];
                        te[u][1] = te[v][1];  //マークを入れ替える
                        te[v][1] = tmp;
                    }
                    if(te[u][0] == te[v][0] && te[u][1] > te[v][1]){  //もし札の数字が同じだった場合にはマークごとに並び替える
                        tmp = te[u][1];
                        te[u][1] = te[v][1];
                        te[v][1] = tmp;
                    }
                }
            }
            for(u = 0;u < 5;u++){
                kari[u][0] = 0;
                kari[u][1] = 0;
            }
            for(int one = 0;one < 6;one++){                             //計10枚の札で最も強い役ができる手札を探す、全探索(物理)
                kari[0][0] = te[one][0];
                kari[0][1] = te[one][1];
                for(int two = one + 1;two < 7;two++){
                    kari[1][0] = te[two][0];
                    kari[1][1] = te[two][1];
                    for(int three = two + 1;three < 8;three++){
                        kari[2][0] = te[three][0];
                        kari[2][1] = te[three][1];
                        for(int four = three + 1;four < 9;four++){
                            kari[3][0] = te[four][0];
                            kari[3][1] = te[four][1];
                            for(int five = four + 1;five < 10;five++){
                                kari[4][0] = te[five][0];
                                kari[4][1] = te[five][1];
                                nowpoint = judge(kari);
                                if(nowpoint > maxpoint){
                                    maxpoint = nowpoint;
                                    saituyo[0][0] = te[one][0];
                                    saituyo[0][1] = te[one][1];
                                    saituyo[1][0] = te[two][0];
                                    saituyo[1][1] = te[two][1];
                                    saituyo[2][0] = te[three][0];
                                    saituyo[2][1] = te[three][1];
                                    saituyo[3][0] = te[four][0];
                                    saituyo[3][1] = te[four][1];
                                    saituyo[4][0] = te[five][0];
                                    saituyo[4][1] = te[five][1];
                                }
                            }
                        }
                    }
                }
            }
            change = 5;
            for(u = 0;u < 5;u++){                       //kohoとsaituyoの確認
                for(int v = 0;v < 5;v++){
                    if(saituyo[v][0] == chand[u][0] && saituyo[v][1] == chand[u][1]){ //kohoとsaituyoが同じ札だった場合
                        mi[u] = 1;
                        change--;
                    }
                }
            }
            for(u = 0;u < 5;u++){
                if(mi[u] == 0){
                    int num,mark;                        //札の数字とマーク
                    num = chand[u][0];
                    mark = chand[u][1];
                    yama[num - 1][mark] = num;
                }
            }
            for(u = 0;u < 5;u++){
                hand[u][0] = saituyo[u][0];
                hand[u][1] = saituyo[u][1];
            }
            break;
    }
    if(change == 0){
        printf("相手は手札を交換しませんでした\n");
    }else{
        printf("相手が手札を%d枚交換しました\n", change);
    }
    return;
}

void syokika(int huda[][2]){
    for(int mu = 0;mu < 52;mu++){
        huda[mu][0] = 0;
        huda[mu][1] = 0;
    }
    return;
}

int main(){
    int mypoint, enemypoint, menu, chip, kai, bet, bai;
    int myhand[5][2];               //自分の手札
    int enemyhand[5][2];            //相手の手札
    int change;                     //入れ替えたい手札
    int deck[13][4];                //山札
    int sutehuda[52][2];            //捨て札
    int before[5][2],after[5][2];   //相手の手札の動きを確認できる
    int level;                      //相手の強さ
    int use = 52;                   //捨て場の枚数
    printf("----------------------------------------------\n");
    printf("|ポーカーを起動しました　　　　　　　　　　　|\n");
    printf("|このゲームは数字の入力のみで動きます　　　　|\n");
    printf("----------------------------------------------\n");
    while(menu != 1){
        printf("数字を入力してください　0:ゲーム終了 1:ゲームスタート 2:役の確認 3:ルールや手順の確認\n");
        scanf("%d", &menu);  //menuを使って動作を判定
        switch(menu){
            case 0:
                printf("ゲームが終了されました\n");
                return 0;
            case 1:
                printf("ゲームを開始します\n");
                break;
            case 2:
                printf("役名                   オッズ(倍率) -役の説明\n");
                printf("ロイヤルストレートフラッシュ 20倍\n -5枚の手札の数字が13,12,11,10,1で5枚が全て同じマーク\n");
                printf("ストレートフラッシュ        10倍\n -連続した5つの数字で5枚全てが同じマーク\n");
                printf("フォーカード               8倍\n -同じ数字を4枚揃える\n");
                printf("フルハウス                 7倍\n -5枚の手札でスリーカードとワンペアを作る\n");
                printf("フラッシュ                 5倍\n -5枚の手札全てを同じマークに揃える(この役のみ役の強弱はスートでつける\n");
                printf("ストレート                 4倍\n -5枚の手札の数字が全て連続している\n");
                printf("スリーカード   　　　       3倍\n -同じ数字を3枚揃sえる\n");
                printf("ツーペア                  2倍\n -同じ数字のペアを二つ作る\n");
                printf("ワンペア                  1倍\n -同じ数字のペアを一つ作る\n");
                continue;
            case 3:
                printf("このゲームはクローズドポーカーです\n");
                printf("クローズドポーカーとは互いの5枚の手札それぞれで役を作り、その役の強さを競うゲームです\n");
                printf("このゲームではまず初期資金の設定を行います、負けるのが不安な人はなしで始めるのがおすすめです\n");
                printf("そして次に弱い、普通、強いの3種類からCPUの強さを選択します、\n");
                printf("すると、プレイヤーとCPUに手札が配られます。このときにベットする金額を入力します\n");
                printf("プレイヤーとCPUが手札を交換した後に、お互いの手札をオープン、手札の役で勝負です\n");
                printf("もし役が同じだった場合は、役を構成する数字の大きい方の勝利です\n");
                printf("勝ったならベットした金額に、役に応じた倍率を掛けた金額が渡されます\n");
                printf("負けた場合は逆にその金額を支払うことになるので、ベットは見極めをしっかりしましょう\n");
                continue;
            default:
                printf("ちゃんと選んでください\n");
                continue;
        }
        printf("\n");
    }
    srand((int)time(NULL));
    kai = 1;  //何回目の試合かを表す
    printf("初期資金はどうしますか？\n");
    while(1){
        printf(" 0:なし 1以上:入力した値がそのままあなたのチップとなります\n");
        scanf("%d", &chip);
        if(chip > 0){
            printf("チップは%d枚から始まります\n", chip);
            break;
        }else if(chip == 0){
            printf("チップはなしで始まります\n");
            break;
        }else{
            printf("ちゃんと入力してください\n");
        }
    }
    while(1){                               //相手の強さの設定
        printf("相手の強さを選んでください 1:弱い 2:普通 3:強い\n");
        scanf("%d", &level);
        switch(level){
            case 1:
                printf("相手の強さを弱いに設定しました\n");
                break;
            case 2:
                printf("相手の強さを普通に設定しました\n");
                break;
            case 3:
                printf("相手の強さを強いに設定しました\n");
                break;
            default:
                printf("ちゃんと選んでください\n");
                continue;
        }
        break;
    }
    while(1){                           //連続でゲームがプレイできるようにループさせる
        printf("第%d回目のゲームを始めます\n", kai);
        if(40 < use){
            if(1 < kai){
                printf("山札が10枚以下となったので");
            }
            printf("山札のシャッフルを行いました\n");
            shuffle(deck);
            syokika(sutehuda);
            use = 0;
        }
        for(int first = 0;first < 5;first++){  //最初のドロー
            drow(first,deck,myhand);           //ドロー処理
            drow(first,deck,enemyhand);
        }
        sort(myhand);                          //手札の並べ替え
        sort(enemyhand);
        save(enemyhand,before);
        open(5,myhand);                           //手札の表示
        while(chip != 0){
            printf("ベットするチップの枚数を入力してください 残りチップ: %d枚\n", chip);
            scanf("%d", &bet);
            if(bet == 0){
                printf("それでは賭け金を用意した意味がありませんよ？\n");
            }else if(bet < 0){
                printf("そんなベットは認められませんよ？\n");
            }else if(chip - bet >= 0){
                printf("ベット: %d\n", bet);
                break;
            }else{
                printf("それでは負債となってしまいますよ？\n");
            }
            printf("今度はちゃんと入力してください\n");
        }
        printf("それではどうしますか？\n");
        while(1){
            printf("0:チェック(交換なし) 1:ドロー(カード交換) 2:捨て札の確認\n");   //ドローするか否か選択
            scanf("%d", &menu);
            if(menu == 0 || menu == 1){
                break;
            }else if(menu == 2){
                if(use == 0){
                    printf("捨て札は何もありません\n");
                }else{
                    trash(sutehuda,use);
                    printf("捨て札の枚数は%d枚です\n", use);
                }
            }else{
                printf("ちゃんと入力してください\n");
            }
        }
        if(menu == 1){
            int mai;
            while(1){                        //正しい数字が入力されるまで繰り返すため
                printf("交換する枚数を選んでください(5枚以内)\n");
                scanf("%d", &mai);
                if(1 <= mai && mai <= 5){
                    break;
                }
                printf("その枚数は交換できません\n");
            }
            int zumi[5] = {0,0,0,0,0};       //交換する手札が交換済みかどうか判定する
            int o = 0;
            while(o < mai){
                printf("%dつ目に交換したい手札は何枚目ですか？(一番左側のカードを1枚目と数えて下さい)\n",o + 1);
                scanf("%d", &change);        //交換する手札を入力
                change--;                    //手札の配列が0からなので-1
                if(change < 0 || 4 < change ){
                    printf("その手札は交換できません\n");
                    continue;
                }
                if(zumi[change] == 1){
                    printf("その手札はすでに選択済みです\n");
                    continue;
                }
                drow(change,deck,myhand);    //交換処理
                zumi[change] = 1;
                o++;
            }
            sort(myhand);
            printf("交換が完了しました\n");
        }else{
            printf("交換はしません\n");
        }
        open(5,myhand);
        printf("相手のターンです\n");
        enemy(deck,enemyhand,level);              //相手の挙動の処理
        sort(enemyhand);
        save(enemyhand,after);
    /*  enemyhand[0][0] = 10;
        enemyhand[0][1] = 3;
        enemyhand[1][0] = 9;
        enemyhand[1][1] = 2;
        enemyhand[2][0] = 8;
        enemyhand[2][1] = 1;
        enemyhand[3][0] = 8;
        enemyhand[3][1] = 2;
        enemyhand[4][0] = 8;  //デバッグ用 
        enemyhand[4][1] = 3; */
        open(5,enemyhand);                    //相手の手札の表示
        bai = 1;                            //できた役に対して何倍のリターンになるかを表す
        printf("自分: ");
        mypoint = judge(myhand);            //自分の手札の役の判定
        yaku(mypoint);
        printf("相手: ");
        enemypoint = judge(enemyhand);      //相手の手札の役の判定
        yaku(enemypoint);
        if(mypoint > enemypoint){
            printf(" YOU WIN !!\n おめでとうございます！\n");
            if(chip != 0){                  //最初のチップ数入力で0と入力していた場合は不要なため
                bai = get(mypoint);
                chip += bet * bai;
                printf("%d枚のチップが増えました\n", bet * bai);
            }
        }else if(mypoint < enemypoint){
            printf(" YOU LOSE !!\n 残念！次は勝ちましょう！\n");
            if(chip != 0){                  //最初のチップ数入力で0と入力していた場合は不要なため
                bai = get(enemypoint);
                chip -= bet * bai;
                printf("%d枚のチップが減ってしまいました\n", bet * bai);
                if(chip <= 0){
                    printf("おやチップが無くなってしまいました\nこれではゲームの継続が不可能ですね\n");
                    break;
                }
            }
        }else{
            printf(" DROW !!\n 惜しかったですね！\n");
        }
        printf("相手の交換した手札の動きを確認しますか？ 0:いいえ 0以外:はい\n");
        scanf("%d", &menu);
        if(menu != 0){
            open(5,before);
            open(5,after);
        }
        printf("現在のチップ: %d\n", chip);
        printf("まだゲームを続けますか？ 0:いいえ 0以外:はい\n");
        scanf("%d", &menu);
        if(menu == 0){
            break;
        }
        kai++;
        for(int y = 0;y < 13;y++){              //使用済みのカードの枚数を調べる
            for(int z = 0;z < 4;z++){
                if(deck[y][z] == 0){
                    sutehuda[use][0] = y + 1;
                    sutehuda[use][1] = z;
                    use++;
                }
            }
        }
    }
    printf("それではゲームを終了します\n");
    return 0;
}