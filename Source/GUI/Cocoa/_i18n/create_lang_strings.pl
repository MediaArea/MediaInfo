#!/usr/bin/perl -w

#tested on Perl v5.12.3
#Author: Max Pozdeev

use Encode qw/encode decode/;
use File::Basename;

my $stringsdb = "./stringsdb.txt";
my $langdbdir = "../Language/";
my $csvdbdir = $langdbdir;
#my $csvdbdir = "../../../Resource/Plugin/Language/"; # Use localized csv from MediaInfo
my $lprojdir = "../";
my $appname = "MediaInfo";

if(int @ARGV == 0) {
    die "Usage: $0 <language>";
}

my $lang = $ARGV[0];
my %db;

my $langdir = $lprojdir."$lang.lproj";
unless(-d $langdir) {
    print "$langdir doesnt exist. Creating...\n";
    mkdir $langdir;
}
my $lang_csv = $csvdbdir."$lang.csv";
unless(-f $lang_csv) {
    die "$lang_csv: csv doesnt exist";
}
my $lang_menu = $langdbdir."Edit.$lang.strings";
unless(-f $lang_menu) {
    die "$lang_menu: menu doesnt exist";
}
my $lang_txt = $langdbdir."$lang.txt";
unless(-f $lang_txt) {
    warn "$lang_txt: txt doesnt exist";
}

#load db
if(-f $stringsdb) {
    #print "Loading db\n";
    open(my $in, '<:raw', $stringsdb) || die "Couldn't open db file: $!";
    my $text = do { local $/; <$in> };
    close $in;
    my $content = decode('UTF-16', $text);
    my @lines = split /\n/, $content;
    foreach(@lines) {
        
        if($_ =~ /^([^=]+)\s*=\s*(.+)/) {
            my $fileparam = $1;
            my $value = $2;
            my($file,$param)=split /\//, $fileparam, 2;
            $db{$file}{$param} = $value;
        }
    }
}

#load menu
my %menu;
if(-f $lang_menu) {
    open(my $in, '<:raw', $lang_menu) || die "Couldn't open menu file: $!";
    my $text = do { local $/; <$in> };
    close $in;
    my $content = decode('UTF-16', $text);
    my @lines = split /\n/, $content;
    foreach(@lines) {
         if($_ =~ /^"(.+?)"\s*=\s*"(.+)"/) {
            $menu{$1} = $2;
         }
    }
    print "Loaded ". (int keys %menu) . " menu keys\n";
}

#load csv
my %csv;
if(-f $lang_csv) {
    open(my $in, '<:raw', $lang_csv) || die "Couldn't open csv file: $!";
    my $text = do { local $/; <$in> };
    close $in;
    my $content = decode('UTF-8', $text);
    $content =~ s/\r\n/\n/g;
    my @lines = split /\n/, $content;
    foreach(@lines) {
         if($_ =~ /^(.+?);\s*(.*)/) {
            $csv{$1} = $2;
         }
    }
    print "Loaded ". (int keys %csv) . " csv keys\n";
}

#load txt
my %txt;
if(-f $lang_txt) {
    open(my $in, '<:raw', $lang_txt) || die "Couldn't open txt file: $!";
    my $text = do { local $/; <$in> };
    close $in;
    my $content = decode('UTF-8', $text);
    $content =~ s/\r\n/\n/g;
    my @lines = split /\n/, $content;
    foreach(@lines) {
         if($_ =~ /^(.+?);\s*(.*)/) {
            $txt{$1} = $2;
         }
    }
    print "Loaded ". (int keys %txt) . " txt keys\n";
}


#go,go,go
foreach(sort keys %db) {
    my $fileIndex = $_;
    my $fileHash = \%{$db{$fileIndex}};
    
    $strings_file = "$langdir/$fileIndex";
    $output_str = "";
    foreach(sort keys %$fileHash) {
        my $param = $_;
        my @values = split(/\//, $fileHash->{$param}, 4);
        
        my $value = $values[3]; #default value
        
        my $found = 0;
        if($values[0] eq 'skip') {
            #print "Skipping $param in $fileIndex\n";
            next;
        }
        elsif($values[0] eq 'csv') {
            if(defined $csv{$values[1]}) {
                $value = $csv{$values[1]};
                #print "csv found $param $value";
                $found = 1;
            }
            else {
                print "csv not found: '$values[1]'\n";
            }
        }
        elsif($values[0] eq 'menu') {
           if(defined $menu{$values[1]}) {
                $value = $menu{$values[1]};
                $found = 1;
            }
            else {
                print "menu not found: '$values[1]'\n";
            }
        }
       elsif($values[0] eq 'txt') {
            if(defined $txt{$values[1]}) {
                $value = $txt{$values[1]};
                $found = 1;
            }
            else {
                print "txt not found: '$values[1]'\n";
            }
        }
        else {
            print "Unknown $values[0]/$values[1] source \n";
        }
        
        if($found && ($values[2] ne '')) { #format
            $value = sprintf($values[2], $value)
        }
        $value =~ s/\\r//g;
        $value =~ s/%%app%%/$appname/g;
        
        $output_str .= "\"$param\" = \"$value\";\n";
    }
    
    open my $out, '>:raw', $strings_file;
    print $out encode("UTF-16", $output_str);
    close $out;

}