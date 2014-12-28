#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRegExpValidator>

#include <core/T.h>

namespace GreenChilli { namespace Components { namespace Playlists {

class AddPlaylistDialog : public QDialog
{
    Q_OBJECT
public:
    QString newPlaylistName() const
    {
        return m_nameEdit->text();
    }

    explicit AddPlaylistDialog( QWidget *parent = 0 );

public slots:
    void clear()
    {
        m_messageLabel->setText( "" );
        m_nameEdit->setText( "" );
    }

private slots:
    void onNameChanged();

private:
    QString m_playlistName;

    QLabel *m_messageLabel;

    QLineEdit *m_nameEdit;

    QPushButton *m_okButton;

    QPushButton *m_cancelButton;
};


} } }
