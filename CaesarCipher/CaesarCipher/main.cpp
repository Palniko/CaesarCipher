#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinbox>
#include <QCheckbox>

#include <string>

// printable ASCII characters
const std::string charset = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
"abcdefghijklmnopqrstuvwxyz{|}~";

std::string CaesarCipher(std::string text, int offset) noexcept {
    for (uint32_t i = 0; i < text.size(); i++)
    {
        if (charset.find(text[i]) != std::string::npos)
        {
            if (charset.find(text[i]) + offset > charset.size())
                if (offset < 0)
                    offset += charset.size();
                else
                    offset -= charset.size();

            text[i] = charset[charset.find(text[i]) + offset];
        }
    }
    return text;
}

void cipher(QTextEdit* input, QTextEdit* output, int offsetValue) {
    output->setText(QString::fromStdString(CaesarCipher(input->toPlainText().toStdString(), offsetValue)));
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("CaesarCipher");
    window.setFixedSize(400, 300);

    QLabel Label1(&window);
    Label1.setGeometry(71, 124, 100, 100);
    Label1.setText("Offset value");

    QSpinBox SpinBoxOffset(&window);
    SpinBoxOffset.setGeometry(15, 165, 50, 20);
    SpinBoxOffset.setValue(3);
    SpinBoxOffset.setRange(0, charset.size());

    QTextEdit inputbox(&window);
    inputbox.setPlaceholderText("Input text...");
    inputbox.setGeometry(0, 0, 200, 160);

    QTextEdit outputbox(&window);
    outputbox.setPlaceholderText("Output text...");
    outputbox.setGeometry(200, 0, 200, 160);
    outputbox.setReadOnly(true);

    QCheckBox CheckBoxLimit("no char limit", &window);
    CheckBoxLimit.setGeometry(135, 125, 100, 100);

    QPushButton buttonEncode("Encodce", &window);
    buttonEncode.setGeometry(15, 200, 100, 75);

    QPushButton buttonDecode("Decode", &window);
    buttonDecode.setGeometry(120, 200, 100, 75);

    QObject::connect(&buttonEncode, &QPushButton::clicked, [&]() {
        cipher(&inputbox, &outputbox, SpinBoxOffset.value());
        });

    QObject::connect(&buttonDecode, &QPushButton::clicked, [&]() {
        cipher(&inputbox, &outputbox, SpinBoxOffset.value() * -1);
        });

    QObject::connect(&inputbox, &QTextEdit::textChanged, [&]() {
        if (CheckBoxLimit.checkState() == false)
        {
            QString text = inputbox.toPlainText().left(255);
            inputbox.blockSignals(true);
            inputbox.setPlainText(text);
            inputbox.blockSignals(false);
            inputbox.moveCursor(QTextCursor::End);
        }
        });

    window.show();
    return app.exec();
}